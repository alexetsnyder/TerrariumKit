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

			float x() const;
			float y() const;
			float z() const;

			std::array<float, 3> id() const;
			glm::vec3 position() const;
			glm::vec3 computeRelativeVoxelPosition(const glm::vec3& worldPos) const;

			bool Equals(const ChunkID& rhs);

		private:
			void setID(float x, float y, float z);
			void setPosition(const glm::vec3& worldPos);

			ChunkSize _chunkSize;
			std::array<float, 3> _id;
	};
}
