#pragma once

#include <glad/glad.h>
#include <SDL/SDL.h>

#include <string>

class Texture
{
	public:
		Texture();
		~Texture();

		void init(const char* filePath);

		void bind() const;

	private:
		SDL_Surface* loadImage(const char* filePath);

		GLuint _id;
};

