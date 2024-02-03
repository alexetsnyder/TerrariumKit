#pragma once 

#include "BlockType.h"
#include "Structs.h"

#include "FastNoiseLite.h"
#include <glm/glm.hpp>
#include <glad/glad.h>

#include <map>

class TerrainGen
{
	public:
		TerrainGen(ChunkSize chunkSize, float minHeight, float varyHeight);

		GLubyte getVoxel(const glm::vec3& position) const;
		BlockType getBlockType(GLubyte byte) const;

	private:
		void createBlockByteLookUp();

		ChunkSize _chunkSize;
		std::map<GLubyte, BlockType> _blockTypeLookUp;
		std::map<std::string, GLubyte> _blockByteLookUp;

		FastNoiseLite _noise;
		float _minHeight;
		float _varyHeight;
};

