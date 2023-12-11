#include "Shape.h"

#include <glad/glad.h>

void Shape::genBuffers()
{
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);
}

void Shape::bindBuffers()
{
	bindVertexArray();
	bindVertexBuffer();
	bindElementBuffer();
}

void Shape::bindVertexArray()
{
	glBindVertexArray(_vao);
}

void Shape::bindVertexBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
}

void Shape::bindElementBuffer()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
}

void Shape::unbindBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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

	if (_ebo != 0)
	{
		glDeleteBuffers(1, &_ebo);
	}
}
