#pragma once

#include "Chunk.h"
#include "WorldGen.h"

class World
{
	public:
		World();

		void init(int worldSize, ChunkSize chunkSize);

	private:
		int _worldSize;
		ChunkSize _chunkSize;

		WorldGen _worldGen;
		Chunk _chunk;
};

