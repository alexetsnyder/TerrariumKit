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
#include <thread>

struct ChunkMeshInfo
{
	Chunk* chunkPointer;
	Mesh chunkMesh;
};

class ChunkManager
{
	public:
		ChunkManager();
		ChunkManager(const World& world, bool useThreading);
		~ChunkManager();

		void init(const World& world, bool useThreading);

		void queueChunks();
		void createChunks(int n);
		void sendChunkData(int n);

		void update();
		void draw(const ShaderProgram& program);

	private:
		bool hasSolidVoxel(const Chunk& chunk, const glm::vec3& position) const;
		bool hasSolidVoxel(const glm::vec3& worldPos) const;

		void createChunk(Chunk& chunk);
		void createVoxel(const Chunk& chunk, const glm::vec3& voxelPosition, Mesh& chunkMesh, int& vertexCount);

		const World* _world;
		TerrainGen _terrainGen;
		std::queue<ChunkID> _chunkIdQueue;

		bool _useThreading;
		std::queue<std::thread> _threadQueue;
		std::queue<ChunkMeshInfo> _chunkMeshInfoQueue;

		std::map<std::array<float, 3>, Chunk> _activeChunkMap;
		std::map<std::array<float, 3>, Chunk> _inactiveChunkMap;
};

