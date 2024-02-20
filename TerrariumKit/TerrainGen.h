#pragma once 

#include "ITerrainGen.h"
#include "Structs.h"
#include "VoxelType.h"

#include <FastNoise/FastNoiseLite.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

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

			ChunkSize chunkSize_;
			std::map<GLubyte, VoxelType> voxelTypeLookUp_;
			std::map<std::string, GLubyte> voxelByteLookUp_;

			FastNoiseLite noise_;
			float minHeight_;
			float varyHeight_;
	};
}
