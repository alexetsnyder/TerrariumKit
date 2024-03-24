#pragma once

#include "MeshRenderer.h"

namespace MemTK
{
	class MeshRendererPool
	{
		public:
			static const int POOL_SIZE{ 300 };

			MeshRendererPool();
			~MeshRendererPool();

			CompTK::MeshRenderer* newMeshRenderer(const glm::vec3& position);
			void deleteMeshRenderer(CompTK::MeshRenderer* renderer);

		private:
			CompTK::MeshRenderer* firstAvailable_;
			CompTK::MeshRenderer pool_[POOL_SIZE];
	};
}
