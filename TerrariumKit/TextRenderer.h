#pragma once

#include "GlyphAtlas.h"
#include "ITextRenderer.h"
#include "QuadRenderer.h"
#include "Texture.h"
#include "ShaderProgram.h"

#include <string_view>

namespace TextTK
{
	class TextRenderer : public ITextRenderer
	{
		public:
			TextRenderer(int fontType, const SDL_Rect& bounds);
			~TextRenderer();
			TextRenderer(const TextRenderer&) = delete;

			//From TextTK::ITextRenderer
			void drawTextLine(std::string_view line, int x, int y) override;
			void drawTextWrapped(std::string_view text, int x, int y) override;
			void draw(const RenderTK::ShaderProgram& program) const override;

		private:
			void drawTextLine(std::string_view line, SDL_Surface* dstSurface, int x, int y);
			void drawTextWrapped(std::string_view text, int x, int y, int maxWidth, int maxHeight);

			int fontType_;
			SDL_Rect bounds_;
			GlyphAtlas atlas_;
			RenderTK::Texture textTexture_;
			RenderTK::QuadRenderer quad_;
	};
}
