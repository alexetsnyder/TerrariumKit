#pragma once

#include "ChunkManager.h"
#include "ICamera.h"
#include "TextElement.h"
#include "Player.h"
#include "ShaderProgram.h"
#include "Time.h"
#include "World.h"

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
		void createShaderPrograms();

		void createCamera();
		void createPlayer();
		void createWorld();
		void createChunkManager();
		void createTextElement();
		
		void gameLoop();
		void pollEvents();
		void updateGame();
		void handleInput();
		void updateText();
		void drawGame();
		void renderText();

		void free();
		void terminate();
		void fatalError();

		SDL_Window* window_;
		int screenWidth_;
		int screenHeight_;

		GameState gameState_;
		RenderTK::ShaderProgram shaderProgram_;

		RenderTK::ShaderProgram textShaderProgram_;
		TextTK::TextElement* debugText_;

		bool drawWireFrame_;
		ProcGenTK::World* world_;
		ProcGenTK::ChunkManager* chunkManager_;

		float cameraSpeed_;
		ICamera* camera_;
		Player* player_;
};
