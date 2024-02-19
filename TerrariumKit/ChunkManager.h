#pragma once

#include "Chunk.h"
#include "ChunkID.h"
#include "IChunkMediator.h"
#include "ITerrainGen.h"
#include "ShaderProgram.h"
#include "TerrainGen.h"
#include "World.h"

#include <glm/glm.hpp>

#include <array>
#include <map>
#include <mutex>
#include <queue>
#include <thread>

namespace ProcGenTK
{
	struct ChunkMeshInfo
	{
		Chunk* chunkPointer;
		Mesh chunkMesh;
	};

	class ChunkManager : public IChunkMediator
	{
		public:
			ChunkManager(const World* world, bool useThreading);
			~ChunkManager();

			bool hasSolidVoxel(const glm::vec3& worldPos) const override;

			void queueChunks();
			void createChunks(int n);
			void createChunkThreads(int n);
			void joinChunkThreads(int n);
			void sendChunkData(int n);

			void update();
			void draw(const ShaderProgram& program);

		private:
			void createChunk(Chunk* chunk);
			void cleanUpChunkThreads();

			const World* world_;
			const ITerrainGen* terrainGen_;
			std::queue<Chunk*> chunkCreateQueue_;

			bool _useThreading;
			std::queue<std::thread> threadQueue_;
			std::mutex chunkMeshInfoAccess_;
			std::queue<ChunkMeshInfo> chunkMeshInfoQueue_;

			std::map<std::array<float, 3>, Chunk*> activeChunkMap_;
			std::map<std::array<float, 3>, Chunk*> inactiveChunkMap_;
	};
}
