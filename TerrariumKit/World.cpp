#include "World.h"

#include "ChunkID.h"

#include <glm/glm.hpp>

#include <iostream>

World::World()
    : _chunkSize{}, _currentChunkId{}
{
	_worldSize = 0;
    _worldHeight = 0;
    _isInfinite = false;
    _camera = nullptr;
    _hasCurrentChunkIdChanged = false;
}

void World::init(const Camera& camera, int worldSize, int worldHeight, ChunkSize chunkSize, bool isInfinite)
{
    _camera = &camera;
	_worldSize = worldSize;
    _worldHeight = worldHeight;
    _isInfinite = isInfinite;
	_chunkSize = chunkSize;
    _currentChunkId.init(_chunkSize, 0.0f, 4.0f, 0.0f);
    _hasCurrentChunkIdChanged = false;
}

int World::getWorldSize() const
{
    return _worldSize;
}

int World::getWorldHeight() const
{
    return _worldHeight;
}

bool World::isInfinite() const
{
    return _isInfinite;
}

ChunkSize World::getChunkSize() const
{
    return _chunkSize;
}

ChunkID World::getCurrentChunkID() const
{
    return _currentChunkId;
}

bool World::hasCurrentChunkIdChanged() const
{
    return _hasCurrentChunkIdChanged;
}

bool World::isOutsideWorld(const glm::vec3& worldPos) const
{
    if (!isInfinite())
    {
        //# of chunks per side depending on the world size
        int dim = _worldSize + _worldSize + 1;

        int boundY = _worldHeight - 1;

        int lowerBoundX = -(dim * _chunkSize.xWidth) / 2 + _chunkSize.xWidth / 2;
        int lowerBoundZ = -(dim * _chunkSize.zWidth) / 2 + _chunkSize.zWidth / 2;
        int upperBoundX = (dim * _chunkSize.xWidth) / 2 + _chunkSize.xWidth / 2;
        int upperBoundZ = (dim * _chunkSize.zWidth) / 2 + _chunkSize.zWidth / 2;

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
    _hasCurrentChunkIdChanged = false;
    glm::vec3 cameraPos = _camera->getPosition();

    ChunkID chunkId{ _chunkSize, cameraPos };

    if (chunkId.getX() != _currentChunkId.getX() || chunkId.getZ() != _currentChunkId.getZ())
    {
        _hasCurrentChunkIdChanged = true;

        std::cout <<"X: " << chunkId.getX() << " Y: " << chunkId.getY() << " Z: " << chunkId.getZ() << std::endl;
        _currentChunkId = chunkId;   
    }
}

