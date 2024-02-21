#include "World.h"

#include "ChunkID.h"

#include <glm/glm.hpp>

#include <iostream>

namespace ProcGenTK
{
    World::World(const ICamera* camera, int worldSize, int worldHeight, ChunkSize chunkSize, bool isInfinite)
        : currentChunkId_{ chunkSize, 0.0f, 4.0f, 0.0f }
    {
        camera_ = camera;
        worldSize_ = worldSize;
        worldHeight_ = worldHeight;
        isInfinite_ = isInfinite;
        chunkSize_ = chunkSize;
        hasCurrentChunkIdChanged_ = false;
    }

    int World::worldSize() const
    {
        return worldSize_;
    }

    int World::worldHeight() const
    {
        return worldHeight_;
    }

    ChunkSize World::chunkSize() const
    {
        return chunkSize_;
    }

    ChunkID World::currentChunkID() const
    {
        return currentChunkId_;
    }

    bool World::isInfinite() const
    {
        return isInfinite_;
    }

    bool World::hasCurrentChunkIdChanged() const
    {
        return hasCurrentChunkIdChanged_;
    }

    bool World::isOutsideWorld(const glm::vec3& worldPos) const
    {
        if (!isInfinite())
        {
            //# of chunks per side depending on the world size
            int dim = worldSize_ + worldSize_ + 1;

            int boundY = worldHeight_ - 1;

            int lowerBoundX = -(dim * chunkSize_.xWidth) / 2 + chunkSize_.xWidth / 2;
            int lowerBoundZ = -(dim * chunkSize_.zWidth) / 2 + chunkSize_.zWidth / 2;
            int upperBoundX = (dim * chunkSize_.xWidth) / 2 + chunkSize_.xWidth / 2;
            int upperBoundZ = (dim * chunkSize_.zWidth) / 2 + chunkSize_.zWidth / 2;

            int x = static_cast<int>(floor(worldPos.x));
            int y = static_cast<int>(floor(worldPos.y));
            int z = static_cast<int>(floor(worldPos.z));

            if (y < 0 || y > boundY ||
                x < lowerBoundX || x > upperBoundX - 1 ||
                z < lowerBoundZ || z > upperBoundZ - 1)
            {
                return true;
            }
        }

        return false;
    }

    void World::update()
    {
        checkCurrentChunk();
    }

    void World::checkCurrentChunk()
    {
        hasCurrentChunkIdChanged_ = false;
        glm::vec3 cameraPos = camera_->transform().position();

        ChunkID chunkId{ chunkSize_, cameraPos };

        if (chunkId.x() != currentChunkId_.x() || chunkId.z() != currentChunkId_.z())
        {
            hasCurrentChunkIdChanged_ = true;

            std::cout << "X: " << chunkId.x() << " Y: " << chunkId.y() << " Z: " << chunkId.z() << std::endl;
            currentChunkId_ = chunkId;
        }
    }

}
