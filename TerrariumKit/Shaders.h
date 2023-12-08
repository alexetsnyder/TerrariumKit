#pragma once

#include <string>

class Shaders
{
	public:
		Shaders();
		~Shaders();

		bool setVertexShader(std::string shaderPath);

		bool setFragmentShader(std::string shaderPath);

		bool compile(char infoLog[512]);
		
		bool link(char infoLog[512]);

		void use();

	private:
		bool setShader(std::string shaderPath, int glShader);
		std::string readFile(std::string filePath);
		bool compile(unsigned int shader, char infoLog[512]);

		unsigned int _program;
		unsigned int _vertex;
		unsigned int _fragment;
};

