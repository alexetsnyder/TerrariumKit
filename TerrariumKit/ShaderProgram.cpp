#include "ShaderProgram.h"
#include "ErrorLog.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

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
		logError("ShaderProgram::setShader", "Error glShader was neither GL_VERTEX_SHADER nor GL_FRAGMENT_SHADER");
		return false;
	}

	return true;
}

bool ShaderProgram::compile()
{
	char infoLog[512];
	if (!compile(_vertex, infoLog))
	{
		logError("ShaderProgram::compile(vertexShader)", infoLog);
		return false;
	}

	if (!compile(_fragment, infoLog))
	{
		logError("ShaderProgram::compile(fragmentShader)", infoLog);
		return false;
	}

	return true;
}

bool ShaderProgram::link()
{
	int success;

	_program = glCreateProgram();
	glAttachShader(_program, _vertex);
	glAttachShader(_program, _fragment);
	glLinkProgram(_program);

	char infoLog[512];
	glGetProgramiv(_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(_program, 512, NULL, infoLog);
		logError("ShaderProgram::link", infoLog);
		return false;
	}

	glDetachShader(_program, _vertex);
	glDetachShader(_program, _fragment);
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
		logError("ShaderProgram::readFile", "Unable to open file: " + filePath);
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
