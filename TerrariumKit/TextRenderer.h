#pragma once

#include "GlyphAtlas.h"
#include "IRenderer.h"
#include "Texture.h"

#include "ShaderProgram.h"

#include <glad/glad.h>

namespace TextTK
{
	class TextRenderer : public RenderTK::IRenderer
	{
		public:
			TextRenderer(int width, int height);
			~TextRenderer();
			TextRenderer(const TextRenderer&) = delete;

			void update() override;
			void draw(const RenderTK::ShaderProgram& program) const override;

		private:
			void calculateModel(int width, int height);
			void sendData();
			void generateAll();
			void bindAll();
			void unbindAll();
			void free() const;

			GlyphAtlas atlas_;
			RenderTK::Texture textTexture_;
			GLuint vao_;
			GLuint vbo_;
			GLuint ebo_;
			glm::mat4 model_;

			int fontType_;
			double coolDownTime_;
			double coolDown_;
	};
}
