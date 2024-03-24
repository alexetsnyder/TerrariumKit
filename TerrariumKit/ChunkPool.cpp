#include "ChunkPool.h"

#include <assert.h>

namespace ProcGenTK
{
	ChunkPool::ChunkPool(const ChunkSize& chunkSize)
		: chunkCount_{ 0 }
	{
		chunks_ = new Chunk[POOL_SIZE];
		firstAvailable_ = &chunks_[0];
		for (int i = 0; i < POOL_SIZE; i++)
		{
			chunks_[i].allocate(chunkSize);

			if (i != POOL_SIZE - 1)
			{
				chunks_[i].setNext(&chunks_[i + 1]);
			}
		}
	}

	ChunkPool::~ChunkPool()
	{
		delete[] chunks_;
	}

	Chunk* ChunkPool::newChunk(const IChunkMediator* chunkMediator, 
							   const ITerrainGen* terrainGen, 
							   CompTK::MeshRenderer* meshRenderer, 
							   glm::vec3 position)
	{
		assert(firstAvailable_ != nullptr);
		chunkCount_++;

		Chunk* newChunk{ firstAvailable_ };
		firstAvailable_ = newChunk->next();

		newChunk->init(chunkMediator, terrainGen, meshRenderer, position);
		return newChunk;
	}

	void ChunkPool::deleteChunk(Chunk* chunk)
	{
		chunkCount_--;
		chunk->setNext(firstAvailable_);
		firstAvailable_ = chunk;
	}
}