#pragma once
#include <glad/glad.h>

class Shape
{
	public:
		virtual void init() = 0;

		virtual void draw() = 0;	

	protected:
		virtual void sendBufferData() = 0;

		void genAll();
		void bindAll();

		void bindVertexArray();
		void bindVertexBuffer();
		void bindElementBuffer();

		void unbindBuffers();
		void unbindVertexArray();

		void deleteAll();

	private:
		GLuint _vao{0};
		GLuint _vbo{0};
		GLuint _ebo{0};
};

