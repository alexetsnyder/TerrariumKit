#pragma once

#include "Structs.h"

#include <glm/glm.hpp>

#include <array>

class ChunkID
{
	public:
		ChunkID();
		ChunkID(ChunkSize chunkSize, float x, float y, float z);
		ChunkID(ChunkSize chunkSize, glm::vec3 worldPos);

		float getX();
		float getY();
		float getZ();

		std::array<float, 3> getID() const;
		glm::vec3 getPosition() const;
		glm::vec3 getRelativeVoxelPosition(const glm::vec3& worldPos) const;
		
		void setID(float x, float y, float z);
		void setPosition(const glm::vec3& worldPos);

		void init(ChunkSize chunkSize, float x, float y, float z);

		bool Equals(const ChunkID& rhs);

	private:
		ChunkSize _chunkSize;
		std::array<float, 3> _id;
};

