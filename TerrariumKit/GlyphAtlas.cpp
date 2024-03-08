#include "GlyphAtlas.h"

#include <assert.h>

namespace TextTK
{
	GlyphAtlas::GlyphAtlas()
		: fonts_{ nullptr }, glyphs_{}, surfaces_{ nullptr }
	{
		addFont(FontType::Px437_IBM_VGA_8x14, "Assets/Fonts/Px437_IBM_VGA_8x14.ttf");
		addFont(FontType::FREE_MONO, "Assets/Fonts/FreeMono.ttf");
		addFont(FontType::UNIFONT, "Assets/Fonts/unifont.ttf");
		addFont(FontType::COURIER_PRIME, "Assets/Fonts/Courier Prime.ttf");
	}

	GlyphAtlas::~GlyphAtlas()
	{
		for (int i = 0; i < FONT_MAX; i++)
		{
			TTF_CloseFont(fonts_[i]);
			SDL_FreeSurface(surfaces_[i]);
		}
	}

	SDL_Rect GlyphAtlas::getGlyph(int fontType, int character) const
	{
		return glyphs_[fontType][character];
	}

	void GlyphAtlas::addFont(int fontType, const char* filePath)
	{
		fonts_[fontType] = TTF_OpenFont(filePath, FONT_SIZE);
		assert(fonts_ != nullptr);

		surfaces_[fontType] = SDL_CreateRGBSurface(0, FONT_SURFACE_SIZE, FONT_SURFACE_SIZE, 32, 0, 0, 0, 0xff);

		SDL_SetColorKey(surfaces_[fontType], SDL_TRUE, SDL_MapRGBA(surfaces_[fontType]->format, 0, 0, 0, 0));

		SDL_Rect dest{};
		SDL_Color white{ getColor(0xff, 0xff, 0xff, 0xff) };
		for (int i = 1; i < MAX_GLYPHS; i++)
		{
			char c[2]
			{
				static_cast<char>(i),
				'\0',
			};

			SDL_Surface* srcSurface = TTF_RenderUTF8_Blended(fonts_[fontType], c, white);

			TTF_SizeText(fonts_[fontType], c, &dest.w, &dest.h);

			if (dest.x + dest.w >= FONT_SURFACE_SIZE)
			{
				dest.x = 0;
				dest.y += dest.h + 1;

				assert(dest.y + dest.h < FONT_SURFACE_SIZE);
			}

			SDL_BlitSurface(srcSurface, nullptr, surfaces_[fontType], &dest);

			glyphs_[fontType][i] = dest;

			SDL_FreeSurface(srcSurface);

			dest.x += dest.w;
		}
	}

	SDL_Color GlyphAtlas::getColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
	{
		SDL_Color color{};
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;
		return color;
	}
}
