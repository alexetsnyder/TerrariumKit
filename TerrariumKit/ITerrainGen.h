#pragma once

#include "VoxelType.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace ProcGenTK
{
	class ITerrainGen
	{
		public:
			virtual ~ITerrainGen() {};
			virtual GLubyte getVoxel(const glm::vec3& position) const = 0;
			virtual VoxelType getVoxelType(GLubyte byte) const = 0;
	};
}