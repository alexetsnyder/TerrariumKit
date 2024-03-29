#pragma once

#include "ChunkID.h"
#include "ICamera.h"

#include <glm/glm.hpp>

namespace ProcGenTK
{
	class World
	{
		public:
			World(const ICamera* camera, int worldSize, int worldHeight, ChunkSize chunkSize, bool isInfinite);

			int worldSize() const;
			int worldHeight() const;
			ChunkSize chunkSize() const;
			ChunkID currentChunkID() const;

			bool isInfinite() const;
			bool hasCurrentChunkIdChanged() const;

			bool isOutsideWorld(const glm::vec3& worldPos) const;

			void update();

		private:
			void checkCurrentChunk();

			int worldSize_;
			int worldHeight_;
			bool isInfinite_;
			ChunkSize chunkSize_;
			const ICamera* camera_;
			ChunkID currentChunkId_;
			bool hasCurrentChunkIdChanged_;
	};
}


