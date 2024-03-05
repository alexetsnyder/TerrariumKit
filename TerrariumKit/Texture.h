#pragma once

#include <glad/glad.h>
#include <SDL/SDL.h>

#include <string>

class Texture
{
	public:
		Texture(const char* filePath);
		Texture(SDL_Surface* surface);
		~Texture();
		Texture(const Texture&) = delete;

		void bind() const;

	private:
		void calibrateTexture();
		SDL_Surface* loadImage(const char* filePath);
		SDL_Surface* convertSurfaceForOpenGL(SDL_Surface* srcSurface);

		GLuint id_;
};

