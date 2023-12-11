#include "Cube.h"

const float cubeVertexArray[24][3] =
{
     //top face
    -0.5f,  0.5f, -0.5f,  //0
    -0.5f,  0.5f,  0.5f,  //1
     0.5f,  0.5f, -0.5f,  //2
     0.5f,  0.5f,  0.5f,  //3
    
     //bottom face
    -0.5f, -0.5f,  0.5f,  //4
    -0.5f, -0.5f, -0.5f,  //5
     0.5f, -0.5f,  0.5f,  //6
     0.5f, -0.5f, -0.5f,  //7
    
     //front face
    -0.5f, -0.5f, -0.5f,  //8
    -0.5f,  0.5f, -0.5f,  //9
     0.5f, -0.5f, -0.5f,  //10
     0.5f,  0.5f, -0.5f,  //11
    
     //back face
     0.5f, -0.5f,  0.5f,  //12
     0.5f,  0.5f,  0.5f,  //13
    -0.5f, -0.5f,  0.5f,  //14
    -0.5f,  0.5f,  0.5f,  //15
    
     //left face
    -0.5f, -0.5f,  0.5f,  //16
    -0.5f,  0.5f,  0.5f,  //17
    -0.5f, -0.5f, -0.5f,  //18
    -0.5f,  0.5f, -0.5f,  //19
    
     //right face
     0.5f, -0.5f, -0.5f,  //20
     0.5f,  0.5f, -0.5f,  //21
     0.5f, -0.5f,  0.5f,  //22
     0.5f,  0.5f,  0.5f,  //23
};

const float cubeIndexArray[6][4] =
{
	 0,  1,  2,  3, //Top Face
	 4,  5,  6,  7, //Bottom Face
	 8,  9, 10, 11, //Front Face
	12, 13, 14, 15, //Back Face
	16, 17, 18, 19, //Left Face
	20, 21, 22, 23, //Right Face
};

void Cube::init()
{
}

void Cube::draw()
{
}

void Cube::sendBufferData()
{
}
