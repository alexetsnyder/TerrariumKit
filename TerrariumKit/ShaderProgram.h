#pragma once

#include <glm/glm.hpp>
#include <string>

class ShaderProgram
{
	public:
		ShaderProgram();
		~ShaderProgram();

		bool setVertexShader(const std::string shaderPath);

		bool setFragmentShader(const std::string shaderPath);

		bool compile(char infoLog[512]);
		
		bool link(char infoLog[512]);

		void use();

		void setUniform(const std::string name, glm::mat4 matrix);

	private:
		bool setShader(const std::string shaderPath, int glShader);
		std::string readFile(const std::string filePath);
		bool compile(unsigned int shader, char infoLog[512]);

		unsigned int _program;
		unsigned int _vertex;
		unsigned int _fragment;
};

