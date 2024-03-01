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
			virtual void sendData(const Mesh& mesh) = 0;
			virtual void draw(const ShaderProgram& program, const glm::vec3& position) const = 0;
	};
}