#include "ChunkPool.h"

namespace ProcGenTK
{
	ChunkPool::ChunkPool()
	{
		chunks_ = new Chunk[POOL_SIZE];
	}

	ChunkPool::~ChunkPool()
	{
		delete[] chunks_;
	}

	Chunk* ChunkPool::newChunk(const IChunkMediator* chunkMediator, 
							   const ITerrainGen* terrainGen, 
							   CompTK::IMeshRenderer* meshRenderer, 
							   glm::vec3 position, ChunkSize chunkSize)
	{
		for (int i = 0; i < POOL_SIZE; i++)
		{
			if (!chunks_[i].isInUse())
			{
				chunks_[i].init(chunkMediator, terrainGen, meshRenderer, position, chunkSize);
				return &chunks_[i];
			}
		}

		return nullptr;
	}

	void ChunkPool::deleteChunk(Chunk* chunk)
	{
		chunk->setInUse(false);
	}
}