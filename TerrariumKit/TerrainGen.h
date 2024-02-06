#pragma once 

#include "VoxelType.h"
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
			VoxelType getVoxelType(GLubyte byte) const override;

		private:
			void createVoxelByteLookUp();

			ChunkSize _chunkSize;
			std::map<GLubyte, VoxelType> _voxelTypeLookUp;
			std::map<std::string, GLubyte> _voxelByteLookUp;

			FastNoiseLite _noise;
			float _minHeight;
			float _varyHeight;
	};
}
