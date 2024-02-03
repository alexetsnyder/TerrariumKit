#pragma once 

#include "BlockType.h"
#include "ITerrainGen.h"
#include "Structs.h"

#include <FastNoise/FastNoiseLite.h>
#include <glm/glm.hpp>
#include <glad/glad.h>

#include <map>

namespace ProcGenTK
{
	class TerrainGen : public ITerrainGen
	{
		public:
			TerrainGen(ChunkSize chunkSize, float minHeight, float varyHeight);

			GLubyte getVoxel(const glm::vec3& position) const override;
			BlockType getBlockType(GLubyte byte) const override;

		private:
			void createBlockByteLookUp();

			ChunkSize _chunkSize;
			std::map<GLubyte, BlockType> _blockTypeLookUp;
			std::map<std::string, GLubyte> _blockByteLookUp;

			FastNoiseLite _noise;
			float _minHeight;
			float _varyHeight;
	};
}
