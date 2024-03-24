#include "MeshRendererPool.h"

#include <assert.h>

namespace MemTK
{
	MeshRendererPool::MeshRendererPool()
	{
		firstAvailable_ = &pool_[0];
		for (int i = 0; i < POOL_SIZE - 1; i++)
		{
			pool_[i].setNext(&pool_[i + 1]);
		}
	}

	MeshRendererPool::~MeshRendererPool()
	{

	}

	CompTK::MeshRenderer* MeshRendererPool::newMeshRenderer(const glm::vec3& position)
	{
		assert(firstAvailable_ != nullptr);

		CompTK::MeshRenderer* newRenderer{ firstAvailable_ };
		firstAvailable_ = firstAvailable_->next();

		newRenderer->init(position);
		return newRenderer;
	}

	void MeshRendererPool::deleteMeshRenderer(CompTK::MeshRenderer* renderer)
	{
		renderer->free();
		renderer->setNext(firstAvailable_);
		firstAvailable_ = renderer;
	}
}