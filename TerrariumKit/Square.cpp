#include "Square.h"

const float squareVertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
};

const unsigned int indices[] = {
    0, 1, 3,    //first triangle
    1, 2, 3,    //second triangle
};

Square::Square()
{

}

Square::~Square()
{
    deleteBuffers();
    
    if (_ebo != 0)
    {
        glDeleteBuffers(1, &_ebo);
    }
}

void Square::init()
{
    genBuffers();
    glGenBuffers(1, &_ebo);
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
    bindBuffers();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    unbindVertexArray();
}
