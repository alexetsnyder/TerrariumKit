#include "Triangle.h"

#include <glad/glad.h>

const float triangleVertices[] = {
	-0.5f, -0.5f, 0.0f, // left  
	 0.5f, -0.5f, 0.0f, // right 
	 0.0f,  0.5f, 0.0f  // top   
};

Triangle::Triangle()
{

}

Triangle::~Triangle()
{
	deleteAll();
}

void Triangle::init()
{
	genAll();
	sendBufferData();
}

void Triangle::draw()
{
	bindVertexArray();
	glDrawArrays(GL_TRIANGLES, 0, 3);
	unbindVertexArray();
}

void Triangle::sendBufferData()
{
	bindAll();

	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unbindVertexArray();
	unbindBuffers();
}
