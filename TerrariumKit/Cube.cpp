#include "Cube.h"

const float cubeVertices[] =
{
    //Front Face
    0.5f,  0.5f,  0.5f,  //0
   -0.5f,  0.5f,  0.5f,  //1
    0.5f, -0.5f,  0.5f,  //2
   -0.5f, -0.5f,  0.5f,  //3

    //Back Face
   -0.5f,  0.5f, -0.5f,  //4
    0.5f,  0.5f, -0.5f,  //5
   -0.5f, -0.5f, -0.5f,  //6
    0.5f, -0.5f, -0.5f,  //7

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

std::vector<std::string> textureNames =
{
    "bedrock",
    "stone",
    "dirt",
    "grassSide",
    "grassTop",
    "riverRock",
    "cobblestone",
    "sand",
};

Cube::Cube()
    : _atlas(256, 16)
{
    _atlas.createAtlas(textureNames);
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
        std::vector<float> textureCoordinates{ _atlas.getTextureCoordinates(getFaceName(face))};
        for (int vertex = 0; vertex < 4; vertex++)
        {
            Vertex newVertex{};

            //3 components in 1 vertex, and 4 vertex in a face: 3 * 4 = 12
            newVertex.position.x = cubeVertices[12 * face + 3 * vertex];
            newVertex.position.y = cubeVertices[12 * face + 3 * vertex + 1];
            newVertex.position.z = cubeVertices[12 * face + 3 * vertex + 2];

            //2 texture coordinates for each vertex
            newVertex.textureCoordinate.u = textureCoordinates[2 * vertex];
            newVertex.textureCoordinate.v = textureCoordinates[2 * vertex + 1];

            cubeMesh.addVertex(newVertex);
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

std::string Cube::getFaceName(int face)
{
    std::string faceName{};

    switch (face)
    {
        case 0:
        case 1:
        case 2:
        case 3:
            faceName = "grassSide"; 
            break;
        case 4:
            faceName = "grassTop";
            break;
        case 5:
            faceName = "dirt";
            break;
    }

    return faceName;
}
