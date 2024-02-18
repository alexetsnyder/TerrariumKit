#include "MainGame.h"

#include "ErrorLog.h"
#include "FirstPersonCamera.h"
#include "FlyingCamera.h"
#include "Input.h"
#include "RotateCameraCommand.h"
#include "ShaderProgram.h"
#include "TopDownCamera.h"
#include "World.h"
#include "ZoomCameraCommand.h"

#include <SDL/SDL_image.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <cstdlib>
#include <chrono>

MainGame::MainGame()
{
	window_ = nullptr;
	screenWidth_ = 1024;
	screenHeight_ = 768;
	gameState_ = GameState::RUNNING;
	drawWireFrame_ = false;

	initSystems();

	createCamera();
	createPlayer();
	createWorld();
	createChunkManager();
}

MainGame::~MainGame()
{
	free();
	terminate();
}

void MainGame::run()
{
	gameLoop();
}

void MainGame::initSystems()
{
	initSDL();

	setGLAttributes();

	createWindow();

	createGLContext();

	initGlad();

	setGLSettings();
	
	createShaderProgram();
}

void MainGame::initSDL()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		logError("SDL_INIT(SDL_INIT_EVERYTHING)", SDL_GetError());
		fatalError();
	}

	SDL_SetRelativeMouseMode(SDL_TRUE);

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		logError("IMG_Init(IMG_INIT_PNG)", IMG_GetError());
		fatalError();
	}
}

void MainGame::setGLAttributes()
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

void MainGame::createWindow()
{
	window_ = SDL_CreateWindow(
		"TerrariumKit",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		screenWidth_,
		screenHeight_,
		SDL_WINDOW_OPENGL
	);

	if (window_ == NULL)
	{
		logError("SDL_CreateWindow", SDL_GetError());
		fatalError();
	}
}

void MainGame::createGLContext()
{
	SDL_GLContext glContext = SDL_GL_CreateContext(window_);
	if (glContext == NULL)
	{
		logError("SDL_GL_CreateContext", SDL_GetError());
		fatalError();
	}
}

void MainGame::initGlad()
{
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		logError("initGlad::gladLoadGLLoader", "Problem initializing glad.");
		fatalError();
	}
}

void MainGame::setGLSettings()
{
	glViewport(0, 0, screenWidth_, screenHeight_);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	if (drawWireFrame_)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}

void MainGame::createShaderProgram()
{
	if (!shaderProgram_.setVertexShader("Shaders/vertex.glsl") ||
		!shaderProgram_.setFragmentShader("Shaders/fragment.glsl") ||
		!shaderProgram_.compile() ||
		!shaderProgram_.link())
	{
		fatalError();
	}
}

void MainGame::createCamera()
{
	glm::vec3 cameraPos = glm::vec3(8.0f, 120.0f, 8.0f);
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	float yaw = -90.0f;
	float pitch = 0.0f;
	float speed = 8.0f;
	float sensititvity = 0.1f;
	float zoom = 45.0f;

	//camera_ = new FlyingCamera{ cameraPos, worldUp, yaw, pitch, speed, sensititvity, zoom };
	camera_ = new FirstPersonCamera{ cameraPos, worldUp, yaw, pitch, speed, sensititvity, zoom };
	//camera_ = new TopDownCamera{ cameraPos, worldUp, speed, zoom };
}

void MainGame::createPlayer()
{
	player_ = new Player(camera_);
}

void MainGame::createWorld()
{
	ChunkSize chunkSize;
	chunkSize.xWidth = 16;
	chunkSize.zWidth = 16;
	chunkSize.height = 16;
	int worldSize = 2;
	int worldHeight = 128;
	bool isInfinite = true;

	world_ = new ProcGenTK::World(camera_, worldSize, worldHeight, chunkSize, isInfinite);
}

void MainGame::createChunkManager()
{
	bool useThreading = false;

	chunkManager_ = new ProcGenTK::ChunkManager(world_, useThreading);
}

void MainGame::gameLoop()
{
	SysTK::Time::start();
	while (gameState_ != GameState::EXIT)
	{
		SysTK::Time::update();

		pollEvents();

		updateGame();

		drawGame();
	}
}

void MainGame::pollEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				gameState_ = GameState::EXIT;
				break;
			default:
				SysTK::Input::processInput(event);
				break;
		}
	}
}

void MainGame::updateGame()
{
	handleKeys();

	player_->update();
	camera_->update();
	world_->update();
	chunkManager_->update();

	SysTK::Input::update();
}

void MainGame::drawGame()
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaderProgram_.use();

	glm::mat4 view{ camera_->viewMatrix() };
	glm::mat4 projection{ 1.0f };
	
	projection = glm::perspective(glm::radians(camera_->zoom()), (float)screenWidth_ / (float)screenHeight_, 0.1f, 1000.0f);
	
	shaderProgram_.setUniform("view", view);
	shaderProgram_.setUniform("projection", projection);

	chunkManager_->draw(shaderProgram_);
	player_->draw(shaderProgram_);

	SDL_GL_SwapWindow(window_);
}

void MainGame::free()
{
	delete camera_;
	delete player_;
	delete world_;
	delete chunkManager_;
}

void MainGame::terminate()
{
	SDL_DestroyWindow(window_);
	IMG_Quit();
	SDL_Quit();
}

void MainGame::fatalError()
{
	free();
	terminate();
	exit(EXIT_FAILURE);
}

void MainGame::handleKeys()
{
	if (SysTK::Input::getKey(SDLK_ESCAPE))
	{
		gameState_ = GameState::EXIT;
	}
}
