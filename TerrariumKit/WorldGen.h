#pragma once

#include "BlockType.h"
#include "Chunk.h"

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <map>

class WorldGen
{
	public:
		WorldGen();
		WorldGen(ChunkSize chunkSize);

		void init(ChunkSize chunkSize);

		GLubyte getVoxel(glm::vec3 position);
		BlockType getBlockType(GLubyte byte);

	private:
		void createBlockByteLookUp();

		ChunkSize _chunkSize;
		std::map<GLubyte, BlockType> _blockTypeLookUp;
		std::map<std::string, GLubyte> _blockByteLookUp;
};

