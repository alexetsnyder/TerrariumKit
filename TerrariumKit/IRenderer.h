#pragma once

#include "ShaderProgram.h"

namespace TextTK
{
	class IRenderer
	{
		public:
			virtual ~IRenderer() {};
			virtual void draw(const ShaderProgram& program) const = 0;
	};
}