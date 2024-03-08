#pragma once

#include "ShaderProgram.h"

namespace EngineTK
{
	class IGameObject
	{
		public:
			virtual ~IGameObject() {};
			virtual void update() = 0;
			virtual void draw(const RenderTK::ShaderProgram& program) = 0;
	};
}
