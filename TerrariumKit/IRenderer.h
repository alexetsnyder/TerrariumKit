#pragma once

#include "ShaderProgram.h"

namespace TextTK
{
	class IRenderer
	{
		public:
			virtual ~IRenderer() {};
			virtual void update() = 0;
			virtual void draw(const ShaderProgram& program) const = 0;
	};
}