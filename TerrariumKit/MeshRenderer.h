#pragma once

#include "IMeshRenderer.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture.h"

#include "glm/glm.hpp"

namespace CompTK
{
	class MeshRenderer : public IMeshRenderer
	{
		public:
			MeshRenderer(const glm::vec3& position);
			~MeshRenderer();
			MeshRenderer(const MeshRenderer&) = delete;

			//From CompTK::IMeshRenderer
			void sendData(const RenderTK::Mesh& mesh) override;
			void draw(const RenderTK::ShaderProgram& program) const override;

		private:
			void generateAll();
			void bindAll();
			void unbindAll();
			void free() const;

			glm::vec3 position_;

			GLuint vao_;
			GLuint vbo_;
			GLuint ebo_;
			GLuint indicesCount_;			
	};
}
