#pragma once

#include <string>

class Shaders
{
	public:
		Shaders();

		bool setShader(std::string shaderPath, int glShader);

		bool compile(char infoLog[512]);
		
		bool link(char infoLog[512]);

		void use();

	private:
		std::string readFile(std::string filePath);
		bool compile(unsigned int shader, char infoLog[512]);

		unsigned int _program;
		unsigned int _vertex;
		unsigned int _fragment;
};

