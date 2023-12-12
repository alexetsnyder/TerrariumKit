#include "MainGame.h"
#include "ShaderProgram.h"

#include <SDL/SDL_image.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

MainGame::MainGame()
{
	_window = nullptr;
	_screenWidth = 1024;
	_screenHeight = 768;
	_gameState = GameState::RUNNING;
	_drawWireFrame = false;
}

void MainGame::run()
{
	initSystems();

	gameLoop();

	terminate();
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

	initShapes();

	initTextures();
}

void MainGame::initSDL()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << "Error::SDL_Init::" << SDL_GetError() << std::endl;
		SDL_Quit();
	}

	if (!IMG_Init(IMG_INIT_PNG))
	{
		std::cout << "Error::IMG_Init::" << SDL_GetError() << std::endl;
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
		std::cout << "Error::SDL_CreateWindow::" << SDL_GetError() << std::endl;
		SDL_Quit();
	}
}

void MainGame::createGLContext()
{
	SDL_GLContext glContext = SDL_GL_CreateContext(_window);
	if (glContext == NULL)
	{
		std::cout << "Error::SDL_GL_CreateContext::" << SDL_GetError() << std::endl;
		SDL_Quit();
	}
}

void MainGame::initGlad()
{
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		std::cout << "Error::gladLoadGLLoader\n";
		SDL_Quit();
	}
}

void MainGame::setGLSettings()
{
	glViewport(0, 0, _screenWidth, _screenHeight);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	if (_drawWireFrame)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}

void MainGame::createShaderProgram()
{
	char infoLog[512];
	if (!_shaderProgram.setVertexShader("Shaders/vertex.glsl") ||
		!_shaderProgram.setFragmentShader("Shaders/fragment.glsl") ||
		!_shaderProgram.compile(infoLog) ||
		!_shaderProgram.link(infoLog))
	{
		std::cout << "Error::CreateShaderProgram::" << infoLog << std::endl;
		SDL_Quit();
	}
}

void MainGame::initShapes()
{
	_triangle.init();
	_square.init();
	_cube.init();
}

void MainGame::initTextures()
{
	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	SDL_Surface* imageSurface = LoadImage("Assets/Textures/Dirt.png");

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageSurface->w, imageSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageSurface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	SDL_FreeSurface(imageSurface);
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();

		drawGame();
	}
}

void MainGame::processInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
		}
	}
}

void MainGame::drawGame()
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_shaderProgram.use();

	glm::mat4 model{ 1.0f };
	glm::mat4 view{ 1.0f };
	glm::mat4 projection{ 1.0f };
	
	model = glm::rotate(model, (SDL_GetTicks() / 1000.0f) * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	projection = glm::perspective(glm::radians(45.0f), (float)_screenWidth / (float)_screenHeight, 0.1f, 100.0f);
	
	_shaderProgram.setUniform("model", model);
	_shaderProgram.setUniform("view", view);
	_shaderProgram.setUniform("projection", projection);
	
	glBindTexture(GL_TEXTURE_2D, _texture);

	//_triangle.draw();
	//_square.draw();
	_cube.draw();

	SDL_GL_SwapWindow(_window);
}

void MainGame::terminate()
{
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

SDL_Surface* MainGame::LoadImage(const char* filePath)
{
	SDL_Surface* sourceSurface = IMG_Load(filePath);
	SDL_Rect imageFrame{ 0, 0, sourceSurface->w, sourceSurface->h };

	uint32_t redMask;
	uint32_t greenMask;
	uint32_t blueMask;
	uint32_t alphaMask;

#if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
	redMask = 0xff000000;
	greenMask = 0x00ff0000;
	blueMask = 0x0000ff00;
	alphaMask = 0x000000ff;
#else
	redMask = 0x000000ff;
	greenMask = 0x0000ff00;
	blueMask = 0x00ff0000;
	alphaMask = 0xff000000;
#endif

	SDL_Surface* targetSurface = SDL_CreateRGBSurface(0, imageFrame.w, imageFrame.h, 32, redMask, greenMask, blueMask, alphaMask);

	SDL_BlitSurface(sourceSurface, &imageFrame, targetSurface, &imageFrame);

	SDL_FreeSurface(sourceSurface);

	return targetSurface;
}
