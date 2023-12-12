#include "ShaderProgram.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>

ShaderProgram::ShaderProgram()
{
	_program = 0;
	_vertex = 0;
	_fragment = 0;
}

ShaderProgram::~ShaderProgram()
{
	if (_program != 0)
	{
		glDeleteProgram(_program);
	}
}

bool ShaderProgram::setVertexShader(const std::string shaderPath)
{
	return setShader(shaderPath, GL_VERTEX_SHADER);
}

bool ShaderProgram::setFragmentShader(const std::string shaderPath)
{
	return setShader(shaderPath, GL_FRAGMENT_SHADER);
}

bool ShaderProgram::setShader(const std::string shaderPath, int glShaderType)
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

bool ShaderProgram::compile(char infoLog[512])
{
	if (compile(_vertex, infoLog) && compile(_fragment, infoLog))
	{
		return true;
	}

	return false;
}

bool ShaderProgram::link(char infoLog[512])
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

void ShaderProgram::use()
{
	glUseProgram(_program);
}

void ShaderProgram::setUniform(const std::string name, glm::mat4 matrix)
{
	GLuint mat4Loc = glGetUniformLocation(_program, name.c_str());
	glUniformMatrix4fv(mat4Loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

std::string ShaderProgram::readFile(const std::string filePath)
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

bool ShaderProgram::compile(unsigned int shader, char infoLog[512])
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
