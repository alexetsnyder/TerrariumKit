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
}
