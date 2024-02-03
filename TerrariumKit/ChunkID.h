#pragma once

#include "Structs.h"

#include <glm/glm.hpp>

#include <array>

namespace ProcGenTK
{
	class ChunkID
	{
		public:
			ChunkID(ChunkSize chunkSize, float x, float y, float z);
			ChunkID(ChunkSize chunkSize, const glm::vec3& worldPos);

			float getX() const;
			float getY() const;
			float getZ() const;

			std::array<float, 3> getID() const;
			glm::vec3 getPosition() const;
			glm::vec3 getRelativeVoxelPosition(const glm::vec3& worldPos) const;

			bool Equals(const ChunkID& rhs);

		private:
			void setID(float x, float y, float z);
			void setPosition(const glm::vec3& worldPos);

			ChunkSize _chunkSize;
			std::array<float, 3> _id;
	};
}
