#pragma once

#include "Chunk.h"
#include "ChunkID.h"
#include "World.h"
#include "TerrainGen.h"
#include "ShaderProgram.h"

#include <glm/glm.hpp>

#include <queue>
#include <array>
#include <map>

class ChunkManager
{
	public:
		ChunkManager();
		ChunkManager(const World& world);

		void init(const World& world);

		void queueChunks();
		void createChunks(int n);

		void update();
		void draw(const ShaderProgram& program);

	private:
		bool hasSolidVoxel(const Chunk& chunk, const glm::vec3& position) const;
		bool hasSolidVoxel(const glm::vec3& worldPos) const;
		void createChunk();
		void createVoxel(const Chunk& chunk, const glm::vec3& voxelPosition, Mesh& chunkMesh, int& vertexCount);

		const World* _world;
		TerrainGen _terrainGen;
		std::queue<ChunkID> _chunkIdQueue;
		std::map<std::array<float, 2>, Chunk> _activeChunkMap;
		std::map<std::array<float, 2>, Chunk> _inactiveChunkMap;
};

