#pragma once

#include "Chunk.h"
#include "ChunkID.h"
#include "ShaderProgram.h"
#include "TerrainGen.h"
#include "World.h"

#include <glm/glm.hpp>

#include <array>
#include <map>
#include <mutex>
#include <queue>
#include <thread>

struct ChunkMeshInfo
{
	Chunk* chunkPointer;
	Mesh chunkMesh;
};

class ChunkManager
{
	public:
		ChunkManager(const World* world, bool useThreading);
		~ChunkManager();

		void queueChunks();
		void createChunks(int n);
		void createChunkThreads(int n);
		void joinChunkThreads(int n);
		void sendChunkData(int n);

		void update();
		void draw(const ShaderProgram& program);

	private:
		bool hasSolidVoxel(const Chunk* chunk, const glm::vec3& position) const;
		bool hasSolidVoxel(const glm::vec3& worldPos) const;

		void createChunk(Chunk* chunk);
		void createVoxel(const Chunk* chunk, const glm::vec3& voxelPosition, Mesh& chunkMesh, int& vertexCount);
		void cleanUpChunkThreads();

		const World* _world;
		TerrainGen _terrainGen;
		std::queue<Chunk*> _chunkCreateQueue;

		bool _useThreading;
		std::queue<std::thread> _threadQueue;
		std::mutex _chunkMeshInfoAccess;
		std::queue<ChunkMeshInfo> _chunkMeshInfoQueue;

		std::map<std::array<float, 3>, Chunk*> _activeChunkMap;
		std::map<std::array<float, 3>, Chunk*> _inactiveChunkMap;
};

