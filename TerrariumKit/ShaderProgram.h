#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <string>
#include <vector>

class ShaderProgram
{
	public:
		ShaderProgram();
		~ShaderProgram();

		bool setVertexShader(const std::string& shaderPath);

		bool setFragmentShader(const std::string& shaderPath);

		bool compile();
		
		bool link();

		void use();

		void setUniform(const std::string name, glm::mat4 matrix);

	private:
		bool setShader(const std::string shaderPath, int glShader);
		std::string readFile(const std::string& filePath);
		bool compile(GLuint shader, std::vector<GLchar>& infoLog);

		GLuint _programID;
		GLuint _vertexID;
		GLuint _fragmentID;
};

