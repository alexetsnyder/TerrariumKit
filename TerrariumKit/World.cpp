#include "World.h"

#include "Mesh.h"
#include "Chunk.h"

#include <glm/glm.hpp>

const glm::vec3 neighbor[] =
{
    glm::vec3( 0.0f,  0.0f, -1.0f),
    glm::vec3( 0.0f,  0.0f,  1.0f),
    glm::vec3(-1.0f,  0.0f,  0.0f),
    glm::vec3( 1.0f,  0.0f,  0.0f),
    glm::vec3( 0.0f,  1.0f,  0.0f),
    glm::vec3( 0.0f, -0.0f,  0.0f),
};

World::World()
	: _chunkSize{}
{
	_worldSize = 0;
}

void World::init(int worldSize, ChunkSize chunkSize)
{
	_worldSize = worldSize;
	_chunkSize = chunkSize;
	_worldGen.init(chunkSize);

	createChunks();
}

void World::createChunks()
{
	createChunk();
}

void World::createVoxel(glm::vec3 position, Mesh& chunkMesh, int& vertexCount)
{
    BlockType blockType{ _worldGen.getBlockType(_worldGen.getVoxel(position)) };

    for (int face = 0; face < 6; face++)
    {
        if (!hasSolidVoxel(position + neighbor[face]))
        {
            std::vector<float> textureCoordinates{ _chunk.getTextureCoordinates(blockType.getBlockSides(), face) };
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
}

bool World::hasSolidVoxel(glm::vec3 position)
{
    /*if (_chunk.isOutsideChunk(position))
    {
        return false;
    }*/

    return false;
}

void World::draw(ShaderProgram shader)
{
	_chunk.draw(shader);
}

void World::createChunk()
{
	_chunk.init(_chunkSize);

    Mesh chunkMesh;

    int vertexCount = 0;
    for (int y = 0; y < _chunkSize.height; y++)
    {
        for (int x = -_chunkSize.xWidth / 2; x < _chunkSize.xWidth / 2; x++)
        {
            for (int z = -_chunkSize.zWidth / 2; z < _chunkSize.zWidth / 2; z++)
            {
                glm::vec3 position{ x, y, z };
                createVoxel(position, chunkMesh, vertexCount);
            }
        }
    }

    _chunk.setChunkMesh(chunkMesh);
}
