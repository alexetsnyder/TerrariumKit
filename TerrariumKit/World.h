#pragma once

#include "Chunk.h"
#include "WorldGen.h"
#include "ShaderProgram.h"

class World
{
	public:
		World();

		void init(int worldSize, ChunkSize chunkSize);

		void createChunks();

		void draw(ShaderProgram shader);

	private:
		void createChunk();

		int _worldSize;
		ChunkSize _chunkSize;

		WorldGen _worldGen;
		Chunk _chunk;
};

