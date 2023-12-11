#pragma once

#include "ShaderProgram.h"
#include "Triangle.h"
#include "Square.h"
#include "Cube.h"

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
		void initSDL();
		void setGLAttributes();
		void createWindow();
		void createGLContext();
		void initGlad();
		void setGLSettings();
		void createShaderProgram();
		void initShapes();

		void gameLoop();
		void processInput();
		void drawGame();
		void terminate();

		SDL_Window* _window;
		int _screenWidth;
		int _screenHeight;

		GameState _gameState;
		ShaderProgram _shaderProgram;

		bool _drawWireFrame;
		Triangle _triangle;
		Square _square;
		Cube _cube;
};

