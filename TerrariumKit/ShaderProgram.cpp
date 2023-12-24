#include "ShaderProgram.h"
#include "ErrorLog.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <vector>

ShaderProgram::ShaderProgram()
{
	_programID = 0;
	_vertexID = 0;
	_fragmentID = 0;
}

ShaderProgram::~ShaderProgram()
{
	if (_programID != 0)
	{
		glDeleteProgram(_programID);
	}
}

bool ShaderProgram::setVertexShader(const std::string& shaderPath)
{
	return setShader(shaderPath, GL_VERTEX_SHADER);
}

bool ShaderProgram::setFragmentShader(const std::string& shaderPath)
{
	return setShader(shaderPath, GL_FRAGMENT_SHADER);
}

bool ShaderProgram::setShader(const std::string shaderPath, int glShaderType)
{
	std::string shaderSourceStr = readFile(shaderPath);
	const char* shaderSource = shaderSourceStr.c_str();

	if (glShaderType == GL_VERTEX_SHADER)
	{
		_vertexID = glCreateShader(glShaderType);
		glShaderSource(_vertexID, 1, &shaderSource, NULL);
	}
	else if (glShaderType == GL_FRAGMENT_SHADER)
	{
		_fragmentID = glCreateShader(glShaderType);
		glShaderSource(_fragmentID, 1, &shaderSource, NULL);
	}
	else
	{
		logError("ShaderProgram::setShader", "Error glShaderType was neither GL_VERTEX_SHADER nor GL_FRAGMENT_SHADER");
		return false;
	}

	return true;
}

bool ShaderProgram::compile()
{
	std::vector<GLchar> infoLog;
	if (!compile(_vertexID, infoLog))
	{
		logError("ShaderProgram::compile(vertexShader)", &infoLog[0]);
		return false;
	}

	if (!compile(_fragmentID, infoLog))
	{
		logError("ShaderProgram::compile(fragmentShader)", &infoLog[0]);
		return false;
	}

	return true;
}

bool ShaderProgram::link()
{
	bool success = true;

	_programID = glCreateProgram();
	glAttachShader(_programID, _vertexID);
	glAttachShader(_programID, _fragmentID);
	glLinkProgram(_programID);
	
	int isLinked;
	glGetProgramiv(_programID, GL_LINK_STATUS, &isLinked);

	if (!isLinked)
	{
		success = false;

		GLint maxLength{ 0 };
		glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(_programID, 512, NULL, &infoLog[0]);

		logError("ShaderProgram::link", &infoLog[0]);
	}

	glDetachShader(_programID, _vertexID);
	glDetachShader(_programID, _fragmentID);
	glDeleteShader(_vertexID);
	glDeleteShader(_fragmentID);

	return success;
}

void ShaderProgram::use() const
{
	glUseProgram(_programID);
}

void ShaderProgram::setUniform(const std::string& name, const glm::mat4& matrix) const
{
	GLuint mat4Loc = glGetUniformLocation(_programID, name.c_str());
	glUniformMatrix4fv(mat4Loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

std::string ShaderProgram::readFile(const std::string& filePath)
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

bool ShaderProgram::compile(GLuint shader, std::vector<GLchar>& infoLog)
{
	glCompileShader(shader);

	GLint isCompiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_FALSE)
	{
		GLint maxLength{ 0 };
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		infoLog.resize(maxLength);
		glGetShaderInfoLog(shader, 512, NULL, &infoLog[0]);

		glDeleteShader(shader);

		return false;
	}

	return true;
}
