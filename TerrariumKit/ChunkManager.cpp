#include "ChunkManager.h"

namespace ProcGenTK
{
    ChunkManager::ChunkManager(const World* world, bool useThreading)
    {
        _world = world;
        _useThreading = useThreading;

        float minHeight{ 32.0f };
        float varyHeight{ 16.0f };
        _terrainGen = new TerrainGen(_world->chunkSize(), minHeight, varyHeight);

        queueChunks();
    }

    ChunkManager::~ChunkManager()
    {
        cleanUpChunkThreads();

        for (auto& pair : _activeChunkMap)
        {
            delete pair.second;
        }

        for (auto& pair : _inactiveChunkMap)
        {
            delete pair.second;
        }

        delete _terrainGen;
    }

    void ChunkManager::queueChunks()
    {
        if (!_activeChunkMap.empty())
        {
            for (const auto& pair : _activeChunkMap)
            {
                _inactiveChunkMap.emplace(pair.first, pair.second);
            }

            for (const auto& pair : _inactiveChunkMap)
            {
                _activeChunkMap.erase(pair.first);
            }
        }

        ChunkID currentChunkId = _world->currentChunkID();
        float startY = 0.0f;
        float endY = static_cast<float>(_world->worldHeight() / _world->chunkSize().height);
        int viewDistanceInChunks = _world->worldSize();
        float startX = currentChunkId.x() - viewDistanceInChunks;
        float endX = currentChunkId.x() + viewDistanceInChunks;
        float startZ = currentChunkId.z() - viewDistanceInChunks;
        float endZ = currentChunkId.z() + viewDistanceInChunks;

        for (float y = startY; y < endY; y++)
        {
            for (float x = startX; x < endX + 1; x++)
            {
                for (float z = startZ; z < endZ + 1; z++)
                {
                    ChunkID chunkId{ _world->chunkSize(), x, y, z };
                    auto mapIter = _inactiveChunkMap.find(chunkId.id());
                    if (mapIter != _inactiveChunkMap.end())
                    {
                        _activeChunkMap.emplace(mapIter->first, mapIter->second);
                        _inactiveChunkMap.erase(mapIter);
                    }
                    else
                    {
                        Chunk* chunkPointer{ new Chunk{ this, _terrainGen, chunkId.position(), _world->chunkSize() } };
                        _activeChunkMap.emplace(chunkId.id(), chunkPointer);
                        _chunkCreateQueue.push(chunkPointer);
                    }
                }
            }
        }
    }

    void ChunkManager::createChunks(int n)
    {
        int count = 0;
        while (!_chunkCreateQueue.empty() && count++ < n)
        {
            Chunk* chunk{ _chunkCreateQueue.front() };
            _chunkCreateQueue.pop();

            createChunk(chunk);
        }
    }

    void ChunkManager::createChunkThreads(int n)
    {
        int count = 0;
        while (!_chunkCreateQueue.empty() && count++ < n)
        {
            Chunk* chunk{ _chunkCreateQueue.front() };
            _chunkCreateQueue.pop();

            std::thread chunkThread{ &ChunkManager::createChunk, this, chunk };
            _threadQueue.push(std::move(chunkThread));
        }
    }

    void ChunkManager::joinChunkThreads(int n)
    {
        int count = 0;
        while (!_threadQueue.empty() && count++ < n)
        {
            _threadQueue.front().join();
            _threadQueue.pop();
        }
    }

    void ChunkManager::sendChunkData(int n)
    {
        int count = 0;
        while (!_chunkMeshInfoQueue.empty() && count++ < n)
        {
            std::lock_guard<std::mutex> lock(_chunkMeshInfoAccess);
            ChunkMeshInfo chunkMeshInfo{ _chunkMeshInfoQueue.front() };
            _chunkMeshInfoQueue.pop();

            if (chunkMeshInfo.chunkMesh.getIndices().empty())
            {
                chunkMeshInfo.chunkPointer->setNoDraw(true);
            }
            else
            {
                chunkMeshInfo.chunkPointer->setChunkMesh(chunkMeshInfo.chunkMesh);
            }
        }
    }

    void ChunkManager::update()
    {
        if (_useThreading)
        {
            joinChunkThreads(16);
            createChunkThreads(16);
        }
        else
        {
            createChunks(4);
        }
        sendChunkData(4);

        if (_world->isInfinite() && _world->hasCurrentChunkIdChanged())
        {
            queueChunks();
        }
    }

    void ChunkManager::draw(const ShaderProgram& program)
    {
        for (auto& pair : _activeChunkMap)
        {
            pair.second->draw(program);
        }
    }

    bool ChunkManager::hasSolidVoxel(const glm::vec3& worldPos) const
    {
        //isOutsideWorld can only return true if no infinite terrain generation
        if (_world->isOutsideWorld(worldPos))
        {
            return false;
        }

        if (worldPos.y < 0 || worldPos.y > _world->worldHeight() - 1)
        {
            return false;
        }

        ChunkID chunkId{ _world->chunkSize(), worldPos };
        glm::vec3 voxelPos{ chunkId.computeRelativeVoxelPosition(worldPos) };

        const auto mapIter = _activeChunkMap.find(chunkId.id());
        if (mapIter != _activeChunkMap.end() && mapIter->second->hasPopulatedVoxelMap())
        {
            return _terrainGen->getVoxelType(mapIter->second->getVoxelByte(voxelPos)).isSolid();
        }
        else
        {
            return _terrainGen->getVoxelType(_terrainGen->getVoxel(worldPos)).isSolid();
        }
    }

    void ChunkManager::createChunk(Chunk* chunk)
    {
        chunk->populateVoxelMap();

        ChunkMeshInfo chunkMeshInfo;
        chunkMeshInfo.chunkPointer = chunk;

        chunk->createChunkMesh(chunkMeshInfo.chunkMesh);

        std::lock_guard<std::mutex> lock(_chunkMeshInfoAccess);
        _chunkMeshInfoQueue.push(chunkMeshInfo);
    }

    void ChunkManager::cleanUpChunkThreads()
    {
        while (!_threadQueue.empty())
        {
            _threadQueue.front().join();
            _threadQueue.pop();
        }
    }
}
