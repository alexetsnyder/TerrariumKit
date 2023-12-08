#include "Shape.h"

#include <glad/glad.h>

void Shape::genBuffers()
{
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
}

void Shape::bindBuffers()
{
	bindVertexArray();
	bindVertexBuffer();
}

void Shape::bindVertexArray()
{
	glBindVertexArray(_vao);
}

void Shape::bindVertexBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
}

void Shape::unbindVertexArray()
{
	glBindVertexArray(0);
}

void Shape::deleteBuffers()
{
	if (_vbo != 0)
	{
		glDeleteBuffers(1, &_vbo);
	}

	if (_vao != 0)
	{
		glDeleteVertexArrays(1, &_vao);
	}
}
