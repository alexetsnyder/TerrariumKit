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
			void sendData(const RenderTK::Mesh& mesh) override {};
			void draw(const RenderTK::ShaderProgram& program) const override {};
	};
}
