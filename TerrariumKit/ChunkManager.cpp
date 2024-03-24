#include "ChunkManager.h"

#include "MeshRenderer.h"
#include "NullMeshRenderer.h"
#include "TerrainGen.h"

namespace ProcGenTK
{
    ChunkManager::ChunkManager(const World* world)
        : world_{ world }, pool_{ world->chunkSize() },
          chunkTexture_{ "Assets/Textures/Atlas.png",
          RenderTK::TextureSettings{ GL_REPEAT, GL_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST } }
    {
        float minHeight{ 32.0f };
        float varyHeight{ 16.0f };
        terrainGen_ = new TerrainGen(world_->chunkSize(), minHeight, varyHeight);

        queueChunks();
    }

    ChunkManager::~ChunkManager()
    {
        delete terrainGen_;
    }

    void ChunkManager::queueChunks()
    {
        setAllChunksInactive();

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
                        Chunk* chunkPointer{ pool_.newChunk(this, terrainGen_, new CompTK::NullMeshRenderer(), chunkId.position()) };
                        activeChunkMap_.emplace(chunkId.id(), chunkPointer);
                        chunkCreateQueue_.push_back(chunkPointer);
                    }
                }
            }
        }

        deleteInactiveChunks();
    }

    void ChunkManager::createChunks(int n)
    {
        int count = 0;
        while (!chunkCreateQueue_.empty() && count++ < n)
        {
            Chunk* chunk{ chunkCreateQueue_.front() };
            chunkCreateQueue_.pop_front();

            createChunk(chunk);
        }
    }

    void ChunkManager::sendChunkData(int n)
    {
        int count = 0;
        while (!chunkMeshInfoQueue_.empty() && count++ < n)
        {
            ChunkMeshInfo chunkMeshInfo{ nextChunkMeshInfo() };
            chunkMeshInfo.chunkPointer->sendChunkMesh(chunkMeshInfo.chunkMesh);
        }
    }

    void ChunkManager::update()
    {
        if (world_->isInfinite() && world_->hasCurrentChunkIdChanged())
        {
            queueChunks();
        }

        createChunks(4);
        sendChunkData(4);
    }

    void ChunkManager::draw(const RenderTK::ShaderProgram& program)
    {
        chunkTexture_.bind();

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

    ChunkMeshInfo ChunkManager::nextChunkMeshInfo()
    {
        ChunkMeshInfo chunkMeshInfo{ chunkMeshInfoQueue_.front() };
        chunkMeshInfoQueue_.pop_front();
        return chunkMeshInfo;
    }

    void ChunkManager::createChunk(Chunk* chunk)
    {
        chunk->populateVoxelMap();

        ChunkMeshInfo chunkMeshInfo;
        chunkMeshInfo.chunkPointer = chunk;

        chunk->createChunkMesh(chunkMeshInfo.chunkMesh);
        if (!chunkMeshInfo.chunkMesh.empty())
        {
            chunk->setMeshRenderer(new CompTK::MeshRenderer(chunk->position()));
        }

        chunkMeshInfoQueue_.push_back(chunkMeshInfo);
    }

    void ChunkManager::setAllChunksInactive()
    {
        if (!activeChunkMap_.empty())
        {
            for (const auto& pair : activeChunkMap_)
            {
                inactiveChunkMap_.emplace(pair.first, pair.second);
            }

            activeChunkMap_.clear();
        }
    }

    void ChunkManager::deleteInactiveChunks()
    {
        for (auto& pair : inactiveChunkMap_)
        {
            bool isFound{ false };
            for (auto& chunkP : chunkCreateQueue_)
            {
                if (chunkP == pair.second)
                {
                    chunkCreateQueue_.remove(chunkP);
                    isFound = true;
                    break;
                }
            }

            if (!isFound)
            {
                for (auto meshInfoIter{ chunkMeshInfoQueue_.begin() }; meshInfoIter != chunkMeshInfoQueue_.end(); meshInfoIter++)
                {
                    if (meshInfoIter->chunkPointer == pair.second)
                    {
                        chunkMeshInfoQueue_.erase(meshInfoIter);
                        break;
                    }
                }
            }

            pool_.deleteChunk(pair.second);
        }

        inactiveChunkMap_.clear();
    }
}
