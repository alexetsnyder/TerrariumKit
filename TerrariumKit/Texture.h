#pragma once

#include <glad/glad.h>
#include <SDL/SDL.h>

#include <string>

struct TextureSettings
{
	GLenum sWrap;
	GLenum tWrap;
	GLenum minFilter;
	GLenum magFilter;
};

class Texture
{
	public:
		Texture(const char* filePath, TextureSettings settings);
		Texture(SDL_Surface* surface, TextureSettings settings);
		~Texture();
		Texture(const Texture&) = delete;

		void bind() const;

	private:
		void calibrateTexture(TextureSettings settings);
		SDL_Surface* loadImage(const char* filePath);
		SDL_Surface* convertSurfaceForOpenGL(SDL_Surface* srcSurface);

		GLuint id_;
};

