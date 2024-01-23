#pragma once

#include "ChunkManager.h"
#include "FirstPersonCamera.h"
#include "ShaderProgram.h"
#include "World.h"

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
		~MainGame();

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
		void initWorld();

		void gameLoop();
		void pollEvents();
		void addKey(SDL_Keycode key);
		void removeKey(SDL_Keycode key);
		void processMouseMotion(SDL_Event event);
		void updateGame();
		void drawGame();

		void terminate();
		void fatalError();

		void handleKeys();
		std::list<SDL_Keycode>::iterator find(SDL_Keycode key, std::list<SDL_Keycode>& keys);

		SDL_Window* _window;
		int _screenWidth;
		int _screenHeight;

		GameState _gameState;
		ShaderProgram _shaderProgram;

		bool _drawWireFrame;
		World _world;
		ChunkManager _chunkManager;

		FirstPersonCamera _camera;
		std::chrono::duration<double> _deltaTime;
		std::chrono::high_resolution_clock::time_point _lastFrame;

		std::list<SDL_Keycode> _keyCodes;
};

