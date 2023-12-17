#include "World.h"

World::World()
	: _chunkSize{}
{
	_worldSize = 0;
}

void World::init(int worldSize, ChunkSize chunkSize)
{
	_worldSize = worldSize;
	_chunkSize = chunkSize;

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
}
