#include "ChunkManager.h"

#include "MeshRenderer.h"

namespace ProcGenTK
{
    ChunkManager::ChunkManager(const World* world, bool useThreading)
    {
        world_ = world;
        _useThreading = useThreading;

        float minHeight{ 32.0f };
        float varyHeight{ 16.0f };
        terrainGen_ = new TerrainGen(world_->chunkSize(), minHeight, varyHeight);

        queueChunks();
    }

    ChunkManager::~ChunkManager()
    {
        cleanUpChunkThreads();

        for (auto& pair : activeChunkMap_)
        {
            delete pair.second;
        }

        for (auto& pair : inactiveChunkMap_)
        {
            delete pair.second;
        }

        delete terrainGen_;
    }

    void ChunkManager::queueChunks()
    {
        if (!activeChunkMap_.empty())
        {
            for (const auto& pair : activeChunkMap_)
            {
                inactiveChunkMap_.emplace(pair.first, pair.second);
            }

            for (const auto& pair : inactiveChunkMap_)
            {
                activeChunkMap_.erase(pair.first);
            }
        }

        ChunkID currentChunkId = world_->currentChunkID();
        float startY = 0.0f;
        float endY = static_cast<float>(world_->worldHeight() / world_->chunkSize().height);
        int viewDistanceInChunks = world_->worldSize();
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
                    ChunkID chunkId{ world_->chunkSize(), x, y, z };
                    auto mapIter = inactiveChunkMap_.find(chunkId.id());
                    if (mapIter != inactiveChunkMap_.end())
                    {
                        activeChunkMap_.emplace(mapIter->first, mapIter->second);
                        inactiveChunkMap_.erase(mapIter);
                    }
                    else
                    {
                        Chunk* chunkPointer{ new Chunk{ this, terrainGen_, new CompTK::MeshRenderer("Assets/Textures/Atlas.png"), chunkId.position(), world_->chunkSize() } };
                        activeChunkMap_.emplace(chunkId.id(), chunkPointer);
                        chunkCreateQueue_.push(chunkPointer);
                    }
                }
            }
        }
    }

    void ChunkManager::createChunks(int n)
    {
        int count = 0;
        while (!chunkCreateQueue_.empty() && count++ < n)
        {
            Chunk* chunk{ chunkCreateQueue_.front() };
            chunkCreateQueue_.pop();

            createChunk(chunk);
        }
    }

    void ChunkManager::createChunkThreads(int n)
    {
        int count = 0;
        while (!chunkCreateQueue_.empty() && count++ < n)
        {
            Chunk* chunk{ chunkCreateQueue_.front() };
            chunkCreateQueue_.pop();

            std::thread chunkThread{ &ChunkManager::createChunk, this, chunk };
            threadQueue_.push(std::move(chunkThread));
        }
    }

    void ChunkManager::joinChunkThreads(int n)
    {
        int count = 0;
        while (!threadQueue_.empty() && count++ < n)
        {
            threadQueue_.front().join();
            threadQueue_.pop();
        }
    }

    void ChunkManager::sendChunkData(int n)
    {
        int count = 0;
        while (!chunkMeshInfoQueue_.empty() && count++ < n)
        {
            std::lock_guard<std::mutex> lock(chunkMeshInfoAccess_);
            ChunkMeshInfo chunkMeshInfo{ chunkMeshInfoQueue_.front() };
            chunkMeshInfoQueue_.pop();

            if (chunkMeshInfo.chunkMesh.getIndices().empty())
            {
                chunkMeshInfo.chunkPointer->setNoDraw(true);
            }
            else
            {
                chunkMeshInfo.chunkPointer->sendChunkMesh(chunkMeshInfo.chunkMesh);
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

        if (world_->isInfinite() && world_->hasCurrentChunkIdChanged())
        {
            queueChunks();
        }
    }

    void ChunkManager::draw(const ShaderProgram& program)
    {
        for (auto& pair : activeChunkMap_)
        {
            pair.second->draw(program);
        }
    }

    bool ChunkManager::hasSolidVoxel(const glm::vec3& worldPos) const
    {
        //isOutsideWorld can only return true if no infinite terrain generation
        if (world_->isOutsideWorld(worldPos))
        {
            return false;
        }

        if (worldPos.y < 0 || worldPos.y > world_->worldHeight() - 1)
        {
            return false;
        }

        ChunkID chunkId{ world_->chunkSize(), worldPos };
        glm::vec3 voxelPos{ chunkId.computeRelativeVoxelPosition(worldPos) };

        const auto mapIter = activeChunkMap_.find(chunkId.id());
        if (mapIter != activeChunkMap_.end() && mapIter->second->hasPopulatedVoxelMap())
        {
            return terrainGen_->getVoxelType(mapIter->second->getVoxelByte(voxelPos)).isSolid();
        }
        else
        {
            return terrainGen_->getVoxelType(terrainGen_->getVoxel(worldPos)).isSolid();
        }
    }

    void ChunkManager::createChunk(Chunk* chunk)
    {
        chunk->populateVoxelMap();

        ChunkMeshInfo chunkMeshInfo;
        chunkMeshInfo.chunkPointer = chunk;

        chunk->createChunkMesh(chunkMeshInfo.chunkMesh);

        std::lock_guard<std::mutex> lock(chunkMeshInfoAccess_);
        chunkMeshInfoQueue_.push(chunkMeshInfo);
    }

    void ChunkManager::cleanUpChunkThreads()
    {
        while (!threadQueue_.empty())
        {
            threadQueue_.front().join();
            threadQueue_.pop();
        }
    }
}
