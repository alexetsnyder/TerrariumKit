#pragma once

#include "GlyphAtlas.h"
#include "IRenderer.h"
#include "Texture.h"

#include "ShaderProgram.h"

#include <glad/glad.h>

namespace TextTK
{
	class TextRenderer : public IRenderer
	{
		public:
			TextRenderer();
			~TextRenderer();
			TextRenderer(const TextRenderer&) = delete;

			void draw(const ShaderProgram& program) const override;

		private:
			void sendData();
			void generateAll();
			void bindAll();
			void unbindAll();
			void free() const;

			GlyphAtlas atlas_;
			Texture textTexture_;
			GLuint vao_;
			GLuint vbo_;
			GLuint ebo_;
	};
}
