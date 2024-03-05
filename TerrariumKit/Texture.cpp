#include "Texture.h"

#include <glad/glad.h>
#include <SDL/SDL_image.h>

Texture::Texture(const char* filePath)
{
	glGenTextures(1, &id_);
	glBindTexture(GL_TEXTURE_2D, id_);

	calibrateTexture();

	SDL_Surface* imageSurface = loadImage(filePath);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageSurface->w, imageSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageSurface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	SDL_FreeSurface(imageSurface);
}

Texture::Texture(SDL_Surface* surface)
{
	glGenTextures(1, &id_);
	glBindTexture(GL_TEXTURE_2D, id_);

	calibrateTexture();

	SDL_Surface* convSurface = convertSurfaceForOpenGL(surface);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, convSurface->w, convSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, convSurface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::~Texture()
{
	glDeleteTextures(1, &id_);
}

void Texture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, id_);
}

void Texture::calibrateTexture()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

SDL_Surface* Texture::loadImage(const char* filePath)
{
	SDL_Surface* srcSurface = IMG_Load(filePath);

	SDL_Surface* targetSurface = convertSurfaceForOpenGL(srcSurface);

	SDL_FreeSurface(srcSurface);

	return targetSurface;
}

SDL_Surface* Texture::convertSurfaceForOpenGL(SDL_Surface* srcSurface)
{
	SDL_Rect srcRect{ 0, 0, srcSurface->w, srcSurface->h };

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

	SDL_Surface* destSurface = SDL_CreateRGBSurface(0, srcRect.w, srcRect.h, 32, redMask, greenMask, blueMask, alphaMask);

	SDL_BlitSurface(srcSurface, &srcRect, destSurface, &srcRect);

	return destSurface;
}
