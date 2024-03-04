#pragma once

#include <SDL/SDL_ttf.h>

#include <string_view>

namespace TextTK
{
	class GlyphAtlas
	{
		public:
			static const int FONT_SURFACE_SIZE{ 512 };
			static const int MAX_GLYPHS{ 128 };
			static const int FONT_SIZE{ 12 };

			GlyphAtlas(const char* filePath);
			~GlyphAtlas();
			GlyphAtlas(const GlyphAtlas&) = delete;

		private:
			void createAtlas(const char* filePath);
			SDL_Color getColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

			TTF_Font* font_;
			SDL_Rect glyphs_[MAX_GLYPHS];
			SDL_Surface* surface_;
	};
}
