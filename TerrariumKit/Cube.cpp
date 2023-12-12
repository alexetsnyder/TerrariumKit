#include "Cube.h"

const float cubeVertices[] =
{
     //Front Face
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  //0
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  //1
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  //2
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  //3

     //Back Face
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  //4
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  //5
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  //6
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  //7

     //Left Face
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  //8
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  //9
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  //10
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  //11

     //Right Face
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  //12
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  //13
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  //14
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  //15

     //Top Face
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  //16
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  //17
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  //18
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  //19

     //Bottom Face
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  //20
    -0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  //21
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  //22
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  //23
};

const unsigned int cubeIndices[] =
{
     0,  1,  2,  2,  1,  3,   //Front Face
     4,  5,  6,  6,  5,  7,   //Back Face
     8,  9, 10, 10,  9, 11,   //Left Face
    12, 13, 14, 14, 13, 15,   //Right Face
    16, 17, 18, 18, 17, 19,   //Top Face
    20, 21, 22, 22, 21, 23,   //Bottom Face
};

Cube::Cube()
{

}

Cube::~Cube()
{
    deleteAll();
}

void Cube::init()
{
    genAll();
    sendBufferData();
}

void Cube::draw()
{
    bindVertexArray();
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    unbindVertexArray();
}

void Cube::sendBufferData()
{
    bindAll();

    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unbindVertexArray();
    unbindBuffers();
}
