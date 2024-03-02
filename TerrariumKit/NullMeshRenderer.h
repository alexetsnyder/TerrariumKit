#pragma once

#include "IMeshRenderer.h"

namespace CompTK
{
	class NullMeshRenderer : public IMeshRenderer
	{
		public:
			NullMeshRenderer() {};
			~NullMeshRenderer() {};

			//From CompTK::IMeshRenderer
			void sendData(const Mesh& mesh) override {};
			void draw(const ShaderProgram& program, const glm::vec3& position) const override {};
	};
}
