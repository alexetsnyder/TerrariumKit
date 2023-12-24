#pragma once

#include "Structs.h"

#include <glm/glm.hpp>

#include <array>

class ChunkID
{
	public:
		ChunkID();
		ChunkID(ChunkSize chunkSize, glm::vec3 worldPos);

		std::array<float, 2> getID() const;
		glm::vec3 getPosition() const;

		void setPosition(const glm::vec3& worldPos);

		void init(ChunkSize chunkSize, glm::vec3 worldPos);

	private:
		ChunkSize _chunkSize;
		std::array<float, 2> _id;
};

