#include "GlyphAtlas.h"

#include <assert.h>

namespace TextTK
{
	GlyphAtlas::GlyphAtlas(const char* filePath)
		: font_{ nullptr }, glyphs_{}, surface_{}
	{
		createAtlas(filePath);
	}

	GlyphAtlas::~GlyphAtlas()
	{
		TTF_CloseFont(font_);
		SDL_FreeSurface(surface_);	
	}

	void GlyphAtlas::createAtlas(const char* filePath)
	{
		font_ = TTF_OpenFont(filePath, FONT_SIZE);

		surface_ = SDL_CreateRGBSurface(0, FONT_SURFACE_SIZE, FONT_SURFACE_SIZE, 32, 0, 0, 0, 0xff);

		SDL_SetColorKey(surface_, SDL_TRUE, SDL_MapRGBA(surface_->format, 0, 0, 0, 0));

		SDL_Rect dest{};
		SDL_Color white{ getColor(0xff, 0xff, 0xff, 0xff) };
		for (int i = 0; i < 128; i++)
		{
			//char c = static_cast<char>(i);
			char c[2]
			{
				static_cast<char>(i),
				'\0',
			};

			SDL_Surface* srcSurface = TTF_RenderUTF8_Blended(font_, c, white);

			TTF_SizeText(font_, c, &dest.w, &dest.h);

			if (dest.x + dest.w >= FONT_SURFACE_SIZE)
			{
				dest.x = 0;
				dest.y = dest.h + 1;

				assert(dest.y + dest.h < FONT_SURFACE_SIZE);
			}

			SDL_BlitSurface(srcSurface, nullptr, surface_, &dest);

			glyphs_[i] = dest;

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
