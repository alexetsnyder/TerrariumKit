#include "Chunk.h"

#include "Mesh.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

std::vector<std::string> blockNames =
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

const float voxelVertices[] =
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
   -0.5f,  0.5f,  0.5f,  //8
   -0.5f,  0.5f, -0.5f,  //9
   -0.5f, -0.5f,  0.5f,  //10
   -0.5f, -0.5f, -0.5f,  //11
   
    //Right Face
    0.5f,  0.5f, -0.5f,  //12
    0.5f,  0.5f,  0.5f,  //13
    0.5f, -0.5f, -0.5f,  //14
    0.5f, -0.5f,  0.5f,  //15

    //Top Face
    0.5f,  0.5f, -0.5f,  //16
   -0.5f,  0.5f, -0.5f,  //17
    0.5f,  0.5f,  0.5f,  //18
   -0.5f,  0.5f,  0.5f,  //19
    
    //Bottom Face
   -0.5f, -0.5f, -0.5f,  //20
    0.5f, -0.5f, -0.5f,  //21
   -0.5f, -0.5f,  0.5f,  //22
    0.5f, -0.5f,  0.5f,  //23 
};

Chunk::Chunk()
    : _atlas{ 256, 16 }, _size{}
{
	_vao = 0;
	_vbo = 0;
	_ebo = 0;
	_position = glm::vec3(0.0f, 0.0f, 0.0f);
    _indicesCount = 0;
}

void Chunk::init(ChunkSize chunkSize)
{
    _size = chunkSize;

    createTextureAtlas();

    Mesh chunkMesh = getChunkMesh();
    setChunkMesh(chunkMesh); 
}

Mesh Chunk::getChunkMesh()
{
    Mesh chunkMesh{};

    int vertexCount = 0;
    for (int y = 0; y < _size.height; y++)
    {
        for (int x = -_size.xWidth / 2; x < _size.xWidth / 2; x++)
        {
            for (int z = -_size.zWidth / 2; z < _size.zWidth / 2; z++)
            {
                glm::vec3 position{ x, y, z };
                createVoxel(position, chunkMesh, vertexCount);
            }
        }
    }

    return chunkMesh;
}

void Chunk::setChunkMesh(Mesh& chunkMesh)
{
    genAll();
    bindAll();

    _indicesCount = chunkMesh.getIndices().size();

    glBufferData(GL_ARRAY_BUFFER, chunkMesh.getVertices().size() * sizeof(Vertex), &chunkMesh.getVertices().front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoordinate));
    glEnableVertexAttribArray(1);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, chunkMesh.getIndices().size() * sizeof(float), &chunkMesh.getIndices().front(), GL_STATIC_DRAW);

    unbindAll();
}

bool Chunk::isOutsideBlock(glm::vec3 position)
{
    int xBound = _size.xWidth / 2;
    int yBound = _size.height - 1;
    int zBound = _size.zWidth / 2;

    int x = static_cast<int>(floor(position.x));
    int y = static_cast<int>(floor(position.y));
    int z = static_cast<int>(floor(position.z));

    if (x < -xBound || x > xBound - 1 ||
        y < 0 || y < yBound ||
        z < -zBound || z > zBound - 1)
    {
        return true;
    }

    return false;
}

bool Chunk::hasSolidBlock(glm::vec3 position)
{
    return false;
}

void Chunk::draw(ShaderProgram shader)
{
    glm::mat4 model{ getModelMatrix() };

    shader.setUniform("model", model);

	_texture.bind();

	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, _indicesCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Chunk::createTextureAtlas()
{
	_atlas.createAtlas(blockNames);
	_texture.init("Assets/Textures/Atlas.png");
}

void Chunk::createVoxel(glm::vec3 position, Mesh& chunkMesh, int& vertexCount)
{
    for (int face = 0; face < 6; face++)
    {
        std::vector<float> textureCoordinates{ _atlas.getTextureCoordinates("stone") };
        for (int vertex = 0; vertex < 4; vertex++)
        {
            Vertex newVertex{};

            //3 components in 1 vertex, and 4 vertex in a face: 3 * 4 = 12
            newVertex.position.x = position.x + voxelVertices[12 * face + 3 * vertex];
            newVertex.position.y = position.y + voxelVertices[12 * face + 3 * vertex + 1];
            newVertex.position.z = position.z + voxelVertices[12 * face + 3 * vertex + 2];

            //2 texture coordinates for each vertex
            newVertex.textureCoordinate.u = textureCoordinates[2 * vertex];
            newVertex.textureCoordinate.v = textureCoordinates[2 * vertex + 1];

            chunkMesh.addVertex(newVertex);
        }

        int indices[] = { 0, 1, 2, 2, 1, 3 };
        for (int i = 0; i < 6; i++)
        {
            chunkMesh.addIndex(vertexCount + indices[i]);
        }

        vertexCount += 4;
    }
}

void Chunk::genAll()
{
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);
}

void Chunk::bindAll()
{
	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
}

void Chunk::unbindAll()
{
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

glm::mat4 Chunk::getModelMatrix()
{
	glm::mat4 model{ 1.0f };
	model = glm::translate(model, _position);
	return model;
}
