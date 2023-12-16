#include "Square.h"

const float squareVertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
    -0.5f,  0.5f, 0.0f,  // top left 
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
};

const unsigned int indices[] = {
    0, 1, 2,    //first triangle
    2, 1, 3,    //second triangle
};

Square::Square()
{

}

Square::~Square()
{
    deleteAll();
}

void Square::init()
{
    genAll();
    sendBufferData();
}

void Square::draw()
{
    bindVertexArray();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    unbindVertexArray();
}

void Square::sendBufferData()
{
    bindAll();

    glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    unbindVertexArray();
    unbindBuffers();
}
