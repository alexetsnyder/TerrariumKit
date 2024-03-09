#pragma once

#include "GlyphAtlas.h"
#include "IRenderer.h"
#include "QuadRenderer.h"
#include "Texture.h"
#include "ShaderProgram.h"

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
			int fontType_;
			std::string& text_;
			GlyphAtlas atlas_;
			RenderTK::Texture textTexture_;
			RenderTK::QuadRenderer quad_;
	};
}
