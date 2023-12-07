#include "Shaders.h"

#include <glad/glad.h>
#include <iostream>
#include <fstream>

Shaders::Shaders()
{
	_program = 0;
	_vertex = 0;
	_fragment = 0;
}

bool Shaders::setVertexShader(std::string shaderPath)
{
	return setShader(shaderPath, GL_VERTEX_SHADER);
}

bool Shaders::setFragmentShader(std::string shaderPath)
{
	return setShader(shaderPath, GL_FRAGMENT_SHADER);
}

bool Shaders::setShader(std::string shaderPath, int glShaderType)
{
	std::string shaderSourceStr = readFile(shaderPath);
	const char* shaderSource = shaderSourceStr.c_str();

	if (glShaderType == GL_VERTEX_SHADER)
	{
		_vertex = glCreateShader(glShaderType);
		glShaderSource(_vertex, 1, &shaderSource, NULL);
	}
	else if (glShaderType == GL_FRAGMENT_SHADER)
	{
		_fragment = glCreateShader(glShaderType);
		glShaderSource(_fragment, 1, &shaderSource, NULL);
	}
	else
	{
		std::cout << "Error glShader was neither GL_VERTEX_SHADER nor GL_FRAGMENT_SHADER\n";
		return false;
	}

	return true;
}

bool Shaders::compile(char infoLog[512])
{
	if (compile(_vertex, infoLog) && compile(_fragment, infoLog))
	{
		return true;
	}

	return false;
}

bool Shaders::link(char infoLog[512])
{
	int success;

	_program = glCreateProgram();
	glAttachShader(_program, _vertex);
	glAttachShader(_program, _fragment);
	glLinkProgram(_program);

	glGetProgramiv(_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(_program, 512, NULL, infoLog);
		return false;
	}

	glDeleteShader(_vertex);
	glDeleteShader(_fragment);

	return true;
}

void Shaders::use()
{
	glUseProgram(_program);
}

std::string Shaders::readFile(std::string filePath)
{
	std::string fileStr = "";
	std::string line;
	std::ifstream file(filePath);

	if (file.is_open())
	{
		while (getline(file, line))
		{
			fileStr += line + "\n";
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file " << filePath << std::endl;
	}

	return fileStr;
}

bool Shaders::compile(unsigned int shader, char infoLog[512])
{
	int success;
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		return false;
	}

	return true;
}
