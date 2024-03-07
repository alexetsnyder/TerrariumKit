#pragma once

#include <SDL/SDL_ttf.h>

#include <string_view>

namespace TextTK
{
	class FontType
	{
		public:
			static const int Px437_IBM_VGA_8x14{ 0 };
			static const int FREE_MONO{ 1 };
			static const int UNIFONT{ 2 };
			static const int COURIER_PRIME{ 3 };
	};

	class GlyphAtlas
	{
		public:
			static const int FONT_SURFACE_SIZE{ 512 };
			static const int MAX_GLYPHS{ 128 };
			static const int FONT_SIZE{ 18 };
			static const int FONT_MAX{ 24 };

			GlyphAtlas(const char* filePath);
			~GlyphAtlas();
			GlyphAtlas(const GlyphAtlas&) = delete;

			SDL_Surface* getSurface(int fontType) const { return surface_[fontType]; }

		private:
			void createAtlas(const char* filePath);
			SDL_Color getColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

			TTF_Font* font_[FONT_MAX];
			SDL_Rect glyphs_[FONT_MAX][MAX_GLYPHS];
			SDL_Surface* surface_[FONT_MAX];
	};
}
