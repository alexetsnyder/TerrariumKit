#pragma once

#include "ShaderProgram.h"
#include "Triangle.h"
#include "Square.h"
#include "Cube.h"
#include "Camera.h"
#include "Texture.h"
#include "Chunk.h"

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
		void pollEvents();
		void addKey(SDL_Keycode key);
		void removeKey(SDL_Keycode key);
		void processMouseMotion(SDL_Event event);
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
		Chunk _chunk;

		Texture _texture;

		Camera _camera;
		std::chrono::duration<double> _deltaTime;
		std::chrono::high_resolution_clock::time_point _lastFrame;

		std::list<SDL_Keycode> _keyCodes;
};

