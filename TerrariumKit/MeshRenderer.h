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
			MeshRenderer();
			~MeshRenderer();
			MeshRenderer(const MeshRenderer&) = delete;

			//From CompTK::IMeshRenderer
			void sendData(const Mesh& mesh) override;
			void draw(const ShaderProgram& program, const glm::vec3& position) const override;

		private:
			void generateAll();
			void bindAll();
			void unbindAll();
			void free() const;

			GLuint vao_;
			GLuint vbo_;
			GLuint ebo_;
			Texture texture_;
			GLuint indicesCount_;			
	};
}
