#pragma once

#include "ShaderProgram.h"
#include "Triangle.h"
#include "Square.h"
#include "Cube.h"
#include "Camera.h"

#include <SDL/SDL.h>
#include <chrono>
#include <list>

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
		void initTextures();

		void gameLoop();
		void processInput();
		void drawGame();
		void terminate();

		void fatalError();

		SDL_Surface* LoadImage(const char* filePath);

		void handleKeys();
		std::list<SDL_Keycode>::iterator find(SDL_Keycode key, std::list<SDL_Keycode>& keys);

		SDL_Window* _window;
		int _screenWidth;
		int _screenHeight;

		GameState _gameState;
		ShaderProgram _shaderProgram;

		bool _drawWireFrame;
		Triangle _triangle;
		Square _square;
		Cube _cube;

		GLuint _texture;

		Camera _camera;
		std::chrono::duration<double> _deltaTime;
		std::chrono::high_resolution_clock::time_point _lastFrame;

		std::list<SDL_Keycode> _keyCodes;
};

