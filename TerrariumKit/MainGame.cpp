#include "MainGame.h"
#include "Shaders.h"

#include <glad/glad.h>
#include <iostream>

MainGame::MainGame()
{
	_window = nullptr;
	_screenWidth = 1024;
	_screenHeight = 768;
	_gameState = GameState::RUNNING;
}

void MainGame::run()
{
	initSystems();

	gameLoop();

	terminate();
}

void MainGame::initSystems()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << "Error::SDL_Init::" << SDL_GetError() << std::endl;
		SDL_Quit();
	}

	setGLAttributes();

	createWindow();

	createGLContext();

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		std::cout << "Error::gladLoadGLLoader\n";
		SDL_Quit();
	}

	glViewport(0, 0, _screenWidth, _screenHeight);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	
	createShaderProgram();

	_triangle.init();
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

void MainGame::createShaderProgram()
{
	char infoLog[512];
	if (!_shaders.setVertexShader("Shaders/vertex.glsl") ||
		!_shaders.setFragmentShader("Shaders/fragment.glsl") ||
		!_shaders.compile(infoLog) ||
		!_shaders.link(infoLog))
	{
		std::cout << "Error::CreateShaderProgram::" << infoLog << std::endl;
		SDL_Quit();
	}
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
	
	glClear(GL_COLOR_BUFFER_BIT);

	_shaders.use();

	_triangle.draw();

	SDL_GL_SwapWindow(_window);
}

void MainGame::terminate()
{
	SDL_DestroyWindow(_window);
	SDL_Quit();
}
