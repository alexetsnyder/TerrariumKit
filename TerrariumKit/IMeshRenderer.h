#pragma once

#include "Mesh.h"
#include "ShaderProgram.h"

#include <glm/glm.hpp>

namespace CompTK
{
	class IMeshRenderer
	{
		public:
			virtual ~IMeshRenderer() {};
			virtual void sendData(const RenderTK::Mesh& mesh) = 0;
			virtual void draw(const RenderTK::ShaderProgram& program) const = 0;
	};
}