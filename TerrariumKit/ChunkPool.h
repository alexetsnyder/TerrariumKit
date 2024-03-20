#pragma once

#include "Chunk.h"

namespace ProcGenTK
{
	class ChunkPool
	{
		public:
			static const int POOL_SIZE{ 600 };

			ChunkPool(const ChunkSize& chunkSize);
			~ChunkPool();
			ChunkPool(const ChunkPool&) = delete;

			Chunk* newChunk(const IChunkMediator* chunkMediator,
							const ITerrainGen* terrainGen,
							CompTK::IMeshRenderer* meshRenderer,
							glm::vec3 position);
			void deleteChunk(Chunk* chunk);

		private:
			int chunkCount_;
			Chunk* firstAvailable_;
			Chunk* chunks_;
	};
}
