#include "Texture.h"

#include <glad/glad.h>
#include <SDL/SDL_image.h>

Texture::Texture()
{
	_id = 0;
}

Texture::~Texture()
{
	glDeleteTextures(1, &_id);
}

void Texture::init(const char* filePath)
{
	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	SDL_Surface* imageSurface = loadImage(filePath);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageSurface->w, imageSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageSurface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	SDL_FreeSurface(imageSurface);
}

void Texture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, _id);
}

SDL_Surface* Texture::loadImage(const char* filePath)
{
	SDL_Surface* sourceSurface = IMG_Load(filePath);
	SDL_Rect imageFrame{ 0, 0, sourceSurface->w, sourceSurface->h };

	uint32_t redMask;
	uint32_t greenMask;
	uint32_t blueMask;
	uint32_t alphaMask;

#if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
	redMask = 0xff000000;
	greenMask = 0x00ff0000;
	blueMask = 0x0000ff00;
	alphaMask = 0x000000ff;
#else
	redMask = 0x000000ff;
	greenMask = 0x0000ff00;
	blueMask = 0x00ff0000;
	alphaMask = 0xff000000;
#endif

	SDL_Surface* targetSurface = SDL_CreateRGBSurface(0, imageFrame.w, imageFrame.h, 32, redMask, greenMask, blueMask, alphaMask);

	SDL_BlitSurface(sourceSurface, &imageFrame, targetSurface, &imageFrame);

	SDL_FreeSurface(sourceSurface);

	return targetSurface;
}
