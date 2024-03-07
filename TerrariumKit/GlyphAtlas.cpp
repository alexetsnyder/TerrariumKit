#include "GlyphAtlas.h"

#include <assert.h>

namespace TextTK
{
	GlyphAtlas::GlyphAtlas(const char* filePath)
		: font_{ nullptr }, glyphs_{}, surface_{ nullptr }
	{
		createAtlas(filePath);
	}

	GlyphAtlas::~GlyphAtlas()
	{
		for (int i = 0; i < FONT_MAX; i++)
		{
			TTF_CloseFont(font_[i]);
			SDL_FreeSurface(surface_[i]);
		}
	}

	void GlyphAtlas::createAtlas(const char* filePath)
	{
		font_[0] = TTF_OpenFont(filePath, FONT_SIZE);
		assert(font_ != nullptr);

		surface_[0] = SDL_CreateRGBSurface(0, FONT_SURFACE_SIZE, FONT_SURFACE_SIZE, 32, 0, 0, 0, 0xff);

		SDL_SetColorKey(surface_[0], SDL_TRUE, SDL_MapRGBA(surface_[0]->format, 0, 0, 0, 0));

		SDL_Rect dest{};
		SDL_Color white{ getColor(0xff, 0xff, 0xff, 0xff) };
		for (int i = 1; i < 128; i++)
		{
			char c[2]
			{
				static_cast<char>(i),
				'\0',
			};

			SDL_Surface* srcSurface = TTF_RenderUTF8_Blended(font_[0], c, white);

			TTF_SizeText(font_[0], c, &dest.w, &dest.h);

			if (dest.x + dest.w >= FONT_SURFACE_SIZE)
			{
				dest.x = 0;
				dest.y += dest.h + 1;

				assert(dest.y + dest.h < FONT_SURFACE_SIZE);
			}

			SDL_BlitSurface(srcSurface, nullptr, surface_[0], &dest);

			glyphs_[0][i] = dest;

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
