#include "MainGame.h"

#include "Camera.h"
#include "ChunkID.h"
#include "ErrorLog.h"
#include "GlyphAtlas.h"
#include "Input.h"
#include "JobManager.h"
#include "Keybindings.h"
#include "TextRenderer.h"
#include "TransformComponent.h"

#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <cstdlib>

MainGame::MainGame()
{
	window_ = nullptr;
	screenWidth_ = 1024;
	screenHeight_ = 768;
	gameState_ = GameState::RUNNING;
	drawWireFrame_ = false;

	initSystems();

	createCamera();
	createWorld();
	createChunkManager();
	createPlayer();
	createTextElement();
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
	
	createShaderPrograms();
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
		logError("IMG_Init(IMG_INIT_PNG)", SDL_GetError());
		fatalError();
	}

	if (TTF_Init() < 0)
	{
		logError("TTF_Init()", SDL_GetError());
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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (drawWireFrame_)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}

void MainGame::createShaderPrograms()
{
	if (!shaderProgram_.setVertexShader("Shaders/vertex.glsl") ||
		!shaderProgram_.setFragmentShader("Shaders/fragment.glsl") ||
		!shaderProgram_.compile() ||
		!shaderProgram_.link())
	{
		fatalError();
	}

	if (!textShaderProgram_.setVertexShader("Shaders/textVertex.glsl") ||
		!textShaderProgram_.setFragmentShader("Shaders/textFragment.glsl") ||
		!textShaderProgram_.compile() ||
		!textShaderProgram_.link())
	{
		fatalError();
	}
}

void MainGame::createCamera()
{
	glm::vec3 cameraPos = glm::vec3(8.0f, 120.0f, 8.0f);
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	float yaw = -90.0f;
	float pitch = -80.0f;

	cameraSpeed_ = 16.0f;
	CompTK::TransformComponent cameraTransform{ cameraPos, worldUp, yaw, pitch };
	float zoom = 45.0f;

	camera_ = new Camera{ cameraTransform, zoom };
}

void MainGame::createPlayer()
{
	float walkSpeed = 8.0f;
	float runSpeed = 16.0f;

	player_ = new Player(camera_, chunkManager_, walkSpeed, runSpeed);
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

void MainGame::createTextElement()
{
	std::string displayStr{ getDebugStr() };

	int fontType = TextTK::FontType::Px437_IBM_VGA_8x14;
	glm::vec3 position{ screenWidth_ / 2.0f, screenHeight_ / 2.0f, 0.0f };
	TextTK::Bounds bounds{ position, screenWidth_, screenHeight_ };

	TextTK::TextRenderer* textRenderer =  new TextTK::TextRenderer{ fontType, bounds };
	debugText_ = new TextTK::TextElement{ displayStr, textRenderer };
}

void MainGame::gameLoop()
{
	constexpr int maxUpdates{ 5 };

	double lag{ 0.0 };

	SysTK::Time::start();
	while (gameState_ != GameState::EXIT)
	{
		SysTK::Time::update();
		lag += SysTK::Time::deltaTime();	

		pollEvents();

		int updateCount{ 0 };
		while (lag >= SysTK::Time::fixedDeltaTime() && updateCount++ < maxUpdates)
		{
			updateGame();
			lag -= SysTK::Time::fixedDeltaTime();
		}

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
	handleInput();

	SysTK::JobManager::update();

	player_->update();
	world_->update();
	chunkManager_->update();
	updateText();

	SysTK::Input::reset();
}

void MainGame::handleInput()
{
	if (SysTK::Input::getKey(SDLK_ESCAPE))
	{
		gameState_ = GameState::EXIT;
	}
}

void MainGame::updateText()
{
	debugText_->updateStr(getDebugStr());
}

std::string MainGame::getDebugStr()
{
	ProcGenTK::ChunkID chunkId{ world_->currentChunkID() };

	std::string displayStr{ "DEBUG INFO:\n" };
	displayStr += "X: " + std::to_string(chunkId.x()) + "\n";
	displayStr += "Y: " + std::to_string(chunkId.y()) + "\n";
	displayStr += "Z: " + std::to_string(chunkId.z()) + "\n";

	return displayStr;
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

	renderText();

	SDL_GL_SwapWindow(window_);
}

void MainGame::renderText()
{
	textShaderProgram_.use();

	glm::mat4 projection{ glm::ortho(0.0f, static_cast<float>(screenWidth_), 0.0f, static_cast<float>(screenHeight_)) };

	textShaderProgram_.setUniform("projection", projection);
	textShaderProgram_.setUniform("textColor", glm::vec3(1.0f, 0.0f, 0.0f));

	debugText_->draw(textShaderProgram_);
}

void MainGame::free()
{
	delete camera_;
	delete player_;
	delete world_;
	delete chunkManager_;
	delete debugText_;
}

void MainGame::terminate()
{
	SDL_DestroyWindow(window_);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void MainGame::fatalError()
{
	free();
	terminate();
	exit(EXIT_FAILURE);
}
