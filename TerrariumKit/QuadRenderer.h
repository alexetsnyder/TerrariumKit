#pragma once

#include "IRenderer.h"
#include "ShaderProgram.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace RenderTK
{
	class QuadRenderer : public IRenderer
	{
		public:
			QuadRenderer(const glm::vec3& position, int width, int height);
			~QuadRenderer();
			QuadRenderer(const QuadRenderer&) = delete;

			//From RenderTK::IRenderer
			void draw(const ShaderProgram& program) const override;

		private:
			void calculateModel(const glm::vec3& position, int width, int height);
			void sendData();
			void generateAll();
			void bindAll();
			void unbindAll();
			void free() const;

			GLuint vao_;
			GLuint vbo_;
			GLuint ebo_;

			glm::mat4 model_;
	};
}
