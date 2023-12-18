#pragma once

#include "Chunk.h"
#include "Mesh.h"
#include "WorldGen.h"
#include "ShaderProgram.h"

#include <glm/glm.hpp>

class World
{
	public:
		World();

		void init(int worldSize, ChunkSize chunkSize);

		void createChunks();
		void createVoxel(glm::vec3 position, Mesh& chunkMesh, int& vertexCount);

		bool hasSolidVoxel(glm::vec3 position);

		void draw(ShaderProgram shader);

	private:
		void createChunk();

		int _worldSize;
		ChunkSize _chunkSize;

		WorldGen _worldGen;
		Chunk _chunk;
};

