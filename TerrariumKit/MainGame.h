#pragma once

#include <SDL/SDL.h>

class MainGame
{
	public:
		MainGame();

		void run();

	private:
		void initSystems();

		SDL_Window* _window;
		int _screenWidth;
		int _screenHeight;
};

