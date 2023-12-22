#pragma once

#include "Chunk.h"
#include "Mesh.h"
#include "WorldGen.h"
#include "ShaderProgram.h"

#include <glm/glm.hpp>

#include <map>
#include <array>

class World
{
	public:
		World();

		void init(int worldSize, ChunkSize chunkSize);

		void createChunks();
		void createVoxel(Chunk& chunk, glm::vec3 position, Mesh& chunkMesh, int& vertexCount);

		bool hasSolidVoxel(Chunk& chunk, glm::vec3 position);
		bool hasSolidVoxel(const glm::vec3& worldPos);

		void update();
		void draw(ShaderProgram shader);

	private:
		void createChunk(glm::vec3 position);
		void createChunkRec(glm::vec3 position, int recDepth);

		int _worldSize;
		ChunkSize _chunkSize;
		WorldGen _worldGen;
		std::map<std::array<float, 3>, Chunk> _activeChunks;
		glm::vec3 _chunkNeighbors[8];
};

