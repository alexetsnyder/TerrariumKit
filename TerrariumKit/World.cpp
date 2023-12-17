#include "World.h"

#include "Mesh.h"

World::World()
	: _chunkSize{}
{
	_worldSize = 0;
}

void World::init(int worldSize, ChunkSize chunkSize)
{
	_worldSize = worldSize;
	_chunkSize = chunkSize;
	_worldGen.init(chunkSize);

	createChunks();
}

void World::createChunks()
{
	createChunk();
}

void World::draw(ShaderProgram shader)
{
	_chunk.draw(shader);
}

void World::createChunk()
{
	_chunk.init(_chunkSize);

    Mesh chunkMesh;

    int vertexCount = 0;
    for (int y = 0; y < _chunkSize.height; y++)
    {
        for (int x = -_chunkSize.xWidth / 2; x < _chunkSize.xWidth / 2; x++)
        {
            for (int z = -_chunkSize.zWidth / 2; z < _chunkSize.zWidth / 2; z++)
            {
                glm::vec3 position{ x, y, z };
                BlockType blockType{ _worldGen.getBlockType(_worldGen.getVoxel(position)) };
                _chunk.createVoxel(blockType, position, chunkMesh, vertexCount);
            }
        }
    }

    _chunk.setChunkMesh(chunkMesh);
}
