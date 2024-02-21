#pragma once

#include <glm/glm.hpp>

namespace ProcGenTK
{
	class IChunkMediator
	{
		public:
			virtual ~IChunkMediator() {};
			virtual bool hasSolidVoxel(const glm::vec3& worldPos) const = 0;
	};
}