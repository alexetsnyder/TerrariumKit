#pragma once

#include "Shaders.h"
#include "Triangle.h"
#include "Square.h"

#include <SDL/SDL.h>

enum GameState
{
	RUNNING,
	EXIT,
};

class MainGame
{
	public:
		MainGame();

		void run();
		
	private:
		void initSystems();
		void setGLAttributes();
		void createWindow();
		void createGLContext();
		void createShaderProgram();

		void gameLoop();
		void processInput();
		void drawGame();
		void terminate();

		SDL_Window* _window;
		int _screenWidth;
		int _screenHeight;

		GameState _gameState;
		Shaders _shaders;

		bool _drawWireFrame;
		Triangle _triangle;
		Square _square;

};

