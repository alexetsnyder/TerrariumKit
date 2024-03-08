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
			static const int FONT_SIZE{ 26 };
			static const int FONT_MAX{ 24 };

			GlyphAtlas();
			~GlyphAtlas();
			GlyphAtlas(const GlyphAtlas&) = delete;

			SDL_Rect getGlyph(int fontType, int character) const; 

			void addFont(int fontType, const char* filePath);

			SDL_Surface* getSurface(int fontType) const { return surfaces_[fontType]; }

		private:
			SDL_Color getColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

			TTF_Font* fonts_[FONT_MAX];
			SDL_Rect glyphs_[FONT_MAX][MAX_GLYPHS];
			SDL_Surface* surfaces_[FONT_MAX];
	};
}
