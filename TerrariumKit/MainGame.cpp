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
	
	createShaderProgram();

	createTriangle();
}

void MainGame::setGLAttributes()
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
}

void MainGame::createWindow()
{
	_window = SDL_CreateWindow(
		"TerrariumKit",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		_screenWidth,
		_screenHeight,
		SDL_WINDOW_SHOWN |
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
	if (!_shaders.setShader("Shaders/vertex.glsl", GL_VERTEX_SHADER) ||
		!_shaders.setShader("Shaders/fragment.glsl", GL_FRAGMENT_SHADER) ||
		!_shaders.compile(infoLog) ||
		!_shaders.link(infoLog))
	{
		std::cout << "Error::CreateShaderProgram::" << infoLog << std::endl;
		SDL_Quit();
	}
}

void MainGame::createTriangle()
{
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left  
		 0.5f, -0.5f, 0.0f, // right 
		 0.0f,  0.5f, 0.0f  // top   
	};

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_shaders.use();

	glBindVertexArray(_vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

	SDL_GL_SwapWindow(_window);
}

void MainGame::terminate()
{
	SDL_DestroyWindow(_window);
	SDL_Quit();
}
