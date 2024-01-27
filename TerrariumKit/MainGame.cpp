#include "MainGame.h"

#include "ErrorLog.h"
#include "FirstPersonCamera.h"
#include "ShaderProgram.h"
#include "TopDownCamera.h"
#include "World.h"

#include <SDL/SDL_image.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <cstdlib>
#include <chrono>

MainGame::MainGame()
{
	_window = nullptr;
	_screenWidth = 1024;
	_screenHeight = 768;
	_gameState = GameState::RUNNING;
	_drawWireFrame = false;
	_deltaTime = std::chrono::duration<double>(0.0);
	_lastFrame = std::chrono::high_resolution_clock::now();

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
		logError("SDL_CreateWindow", SDL_GetError());
		fatalError();
	}
}

void MainGame::createGLContext()
{
	SDL_GLContext glContext = SDL_GL_CreateContext(_window);
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
	glViewport(0, 0, _screenWidth, _screenHeight);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	if (_drawWireFrame)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}

void MainGame::createShaderProgram()
{
	if (!_shaderProgram.setVertexShader("Shaders/vertex.glsl") ||
		!_shaderProgram.setFragmentShader("Shaders/fragment.glsl") ||
		!_shaderProgram.compile() ||
		!_shaderProgram.link())
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

	_camera = new FirstPersonCamera{ cameraPos, worldUp, yaw, pitch, speed, sensititvity, zoom };
	//_camera = new TopDownCamera{ cameraPos, worldUp, speed, zoom };
}

void MainGame::createPlayer()
{
	_player = new Player(_camera);
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

	_world = new World(_camera, worldSize, worldHeight, chunkSize, isInfinite);
}

void MainGame::createChunkManager()
{
	bool useThreading = false;

	_chunkManager = new ChunkManager(_world, useThreading);
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		std::chrono::high_resolution_clock::time_point currentFrame = std::chrono::high_resolution_clock::now();
		_deltaTime = currentFrame - _lastFrame;
		_lastFrame = currentFrame;

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
				_gameState = GameState::EXIT;
				break;
			case SDL_KEYDOWN:
				addKey(event.key.keysym.sym);
				break;
			case SDL_KEYUP:
				removeKey(event.key.keysym.sym);
				break;
			case SDL_MOUSEMOTION:
				processMouseMotion(event);
				break;
			case SDL_MOUSEWHEEL:
				_camera->zoom(static_cast<float>(event.wheel.y));
				break;
		}
	}
}

void MainGame::addKey(SDL_Keycode key)
{
	if (_keyCodes.empty() || find(key, _keyCodes) == _keyCodes.end())
	{
		_keyCodes.push_back(key);
	}
}

void MainGame::removeKey(SDL_Keycode key)
{
	auto keyIter = find(key, _keyCodes);
	if (keyIter != _keyCodes.end())
	{
		_keyCodes.erase(keyIter);
	}
}

void MainGame::processMouseMotion(SDL_Event event)
{
	float xRel = static_cast<float>(event.motion.xrel);
	float yRel = static_cast<float>(event.motion.yrel);
	_camera->rotate(xRel, -yRel);
}

void MainGame::updateGame()
{
	handleKeys();

	_player->update();
	_world->update();
	_chunkManager->update();
}

void MainGame::drawGame()
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_shaderProgram.use();

	glm::mat4 view{ _camera->viewMatrix() };
	glm::mat4 projection{ 1.0f };
	
	projection = glm::perspective(glm::radians(_camera->zoom()), (float)_screenWidth / (float)_screenHeight, 0.1f, 1000.0f);
	
	_shaderProgram.setUniform("view", view);
	_shaderProgram.setUniform("projection", projection);

	_chunkManager->draw(_shaderProgram);
	_player->draw(_shaderProgram);

	SDL_GL_SwapWindow(_window);
}

void MainGame::free()
{
	if (_camera != nullptr)
	{
		delete _camera;
		_camera = nullptr;
	}

	if (_player != nullptr)
	{
		delete _player;
		_player = nullptr;
	}

	if (_world != nullptr)
	{
		delete _world;
		_world = nullptr;
	}

	if (_chunkManager != nullptr)
	{
		delete _chunkManager;
		_chunkManager = nullptr;
	}
}

void MainGame::terminate()
{
	SDL_DestroyWindow(_window);
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
	for (auto key : _keyCodes)
	{
		switch (key)
		{
			case SDLK_ESCAPE:
				_gameState = GameState::EXIT;
				break;
			case SDLK_w:
				//_camera->move(CameraDirection::FORWARD, _deltaTime.count());
				_player->move(PlayerDirection::FORWARD, _deltaTime.count());
				break;
			case SDLK_s:
				//_camera->move(CameraDirection::BACKWARD, _deltaTime.count());
				_player->move(PlayerDirection::BACKWARD, _deltaTime.count());
				break;
			case SDLK_a:
				//_camera->move(CameraDirection::LEFT, _deltaTime.count());
				_player->move(PlayerDirection::LEFT, _deltaTime.count());
				break;
			case SDLK_d:
				//_camera->move(CameraDirection::RIGHT, _deltaTime.count());
				_player->move(PlayerDirection::RIGHT, _deltaTime.count());
				break;
		}
	}
}

std::list<SDL_Keycode>::iterator MainGame::find(SDL_Keycode key, std::list<SDL_Keycode>& keys)
{
	for (std::list<SDL_Keycode>::iterator keyIter = keys.begin(); keyIter != keys.end(); keyIter++)
	{
		if (*keyIter == key)
		{
			return keyIter;
		}
	}

	return keys.end();
}

