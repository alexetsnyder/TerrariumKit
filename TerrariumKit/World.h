#pragma once

#include "ChunkID.h"
#include "Camera.h"

#include <glm/glm.hpp>

class World
{
	public:
		World();

		void init(const Camera& camera, int worldSize, ChunkSize chunkSize);

		int getWorldSize() const;
		ChunkSize getChunkSize() const;
		ChunkID getCurrentChunkID() const;
		bool hasCurrentChunkIdChanged() const;

		bool isOutsideWorld(const glm::vec3& worldPos) const;

		void update();

	private:
		void checkCurrentChunk();

		int _worldSize;
		ChunkSize _chunkSize;
		const Camera* _camera;
		ChunkID _currentChunkId;
		bool _hasCurrentChunkIdChanged;
};

