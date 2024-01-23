#pragma once

#include "ChunkID.h"
#include "ICamera.h"

#include <glm/glm.hpp>

class World
{
	public:
		World();

		void init(const ICamera& camera, int worldSize, int worldHeight, ChunkSize chunkSize, bool isInfinite);

		int getWorldSize() const;
		int getWorldHeight() const;
		bool isInfinite() const;
		ChunkSize getChunkSize() const;
		ChunkID getCurrentChunkID() const;
		bool hasCurrentChunkIdChanged() const;

		bool isOutsideWorld(const glm::vec3& worldPos) const;

		void update();

	private:
		void checkCurrentChunk();

		int _worldSize;
		int _worldHeight;
		bool _isInfinite;
		ChunkSize _chunkSize;
		const ICamera* _camera;
		ChunkID _currentChunkId;
		bool _hasCurrentChunkIdChanged;
};

