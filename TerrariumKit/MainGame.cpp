#include "MainGame.h"

#include <iostream>

MainGame::MainGame()
{
	_window = nullptr;
	_screenWidth = 1024;
	_screenHeight = 768;
}

void MainGame::run()
{
	initSystems();
}

void MainGame::initSystems()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << "Error::SDL_Init::" << SDL_GetError() << std::endl;
	}

	_window = SDL_CreateWindow(
		"TerrariumKit", 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		_screenWidth, 
		_screenHeight, 
		SDL_WINDOW_OPENGL
	);

	if (_window == NULL)
	{
		std::cout << "Error::SDL_CreateWindow::" << SDL_GetError() << std::endl;
	}
}
