#include "Cube.h"

const float cubeVertices[] =
{
    //Back Face
   -0.5f,  0.5f, -0.5f,  //0
    0.5f,  0.5f, -0.5f,  //1
   -0.5f, -0.5f, -0.5f,  //2
    0.5f, -0.5f, -0.5f,  //3

    //Front Face
    0.5f,  0.5f,  0.5f,  //4
   -0.5f,  0.5f,  0.5f,  //5
    0.5f, -0.5f,  0.5f,  //6
   -0.5f, -0.5f,  0.5f,  //7

    //Left Face
   -0.5f,  0.5f, -0.5f,  //8
   -0.5f,  0.5f,  0.5f,  //9
   -0.5f, -0.5f, -0.5f,  //10
   -0.5f, -0.5f,  0.5f,  //11

    //Right Face
    0.5f,  0.5f,  0.5f,  //12
    0.5f,  0.5f, -0.5f,  //13
    0.5f, -0.5f,  0.5f,  //14
    0.5f, -0.5f, -0.5f,  //15

    //Top Face
   -0.5f,  0.5f, -0.5f,  //16
    0.5f,  0.5f, -0.5f,  //17
   -0.5f,  0.5f,  0.5f,  //18
    0.5f,  0.5f,  0.5f,  //19

    //Bottom Face
    0.5f, -0.5f, -0.5f,  //20
   -0.5f, -0.5f, -0.5f,  //21
    0.5f, -0.5f,  0.5f,  //22
   -0.5f, -0.5f,  0.5f,  //23
};

const float textureCoordinates[] =
{
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
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

    Mesh cubeMesh = getCubeMesh();

    glBufferData(GL_ARRAY_BUFFER, cubeMesh.getVertices().size() * sizeof(Vertex), &cubeMesh.getVertices().front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoordinate));
    glEnableVertexAttribArray(1);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeMesh.getIndices().size() * sizeof(float), &cubeMesh.getIndices().front(), GL_STATIC_DRAW);

    unbindVertexArray();
    unbindBuffers();
}

Mesh Cube::getCubeMesh()
{
    Mesh cubeMesh{};

    int vertexCount = 0;
    for (int face = 0; face < 6; face++)
    {
        for (int v = 0; v < 4; v++)
        {
            Vertex vertex{};
            vertex.position.x = cubeVertices[12 * face + 3 * v];
            vertex.position.y = cubeVertices[12 * face + 3 * v + 1];
            vertex.position.z = cubeVertices[12 * face + 3 * v + 2];

            vertex.textureCoordinate.u = textureCoordinates[2 * v];
            vertex.textureCoordinate.v = textureCoordinates[2 * v + 1];

            cubeMesh.addVertex(vertex);
        }

        int indices[] = { 0, 1, 2, 2, 1, 3 };
        for (int i = 0; i < 6; i++)
        {
            cubeMesh.addIndex(vertexCount + indices[i]);
        }

        vertexCount += 4;
    }

    return cubeMesh;
}
