#pragma once

#include "GlyphAtlas.h"
#include "IRenderer.h"
#include "Texture.h"
#include "ShaderProgram.h"

#include <glad/glad.h>

#include <string>
#include <string_view>

namespace TextTK
{
	class TextRenderer : public RenderTK::IRenderer
	{
		public:
			TextRenderer(std::string text, int fontType, int width, int height);
			~TextRenderer();
			TextRenderer(const TextRenderer&) = delete;

			SDL_Surface* drawText(std::string_view text, int x, int y, int width, int height);

			//From RenderTK::IRenderer
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
			std::string& text_;
			int fontType_;

	};
}
