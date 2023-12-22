#include "World.h"

#include "Mesh.h"
#include "Chunk.h"

#include <glm/glm.hpp>

#include <array>
#include <iostream>

const float voxelVertices[] 
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

const glm::vec3 voxelNeighbors[] 
{
    glm::vec3( 0.0f,  0.0f,  1.0f),
    glm::vec3( 0.0f,  0.0f, -1.0f),
    glm::vec3(-1.0f,  0.0f,  0.0f),
    glm::vec3( 1.0f,  0.0f,  0.0f),
    glm::vec3( 0.0f,  1.0f,  0.0f),
    glm::vec3( 0.0f, -1.0f,  0.0f),
};

World::World()
    : _chunkSize{}, _chunkNeighbors{}, _currentChunk{ -1.0f, 0.0f, 0.0f }
{
	_worldSize = 0;
    _camera = nullptr;
}

void World::init(const Camera& camera, int worldSize, ChunkSize chunkSize)
{
    _camera = &camera;

	_worldSize = worldSize;
	_chunkSize = chunkSize;
	_worldGen.init(chunkSize, 32, 16);

    _chunkNeighbors[0] = glm::vec3(-_chunkSize.xWidth, 0.0f, -_chunkSize.zWidth);
    _chunkNeighbors[1] = glm::vec3(              0.0f, 0.0f, -_chunkSize.zWidth);
    _chunkNeighbors[2] = glm::vec3( _chunkSize.xWidth, 0.0f, -_chunkSize.zWidth);
    _chunkNeighbors[3] = glm::vec3(-_chunkSize.xWidth, 0.0f,               0.0f);
    _chunkNeighbors[4] = glm::vec3( _chunkSize.xWidth, 0.0f,               0.0f);
    _chunkNeighbors[5] = glm::vec3(-_chunkSize.xWidth, 0.0f,  _chunkSize.zWidth);
    _chunkNeighbors[6] = glm::vec3(              0.0f, 0.0f,  _chunkSize.zWidth);
    _chunkNeighbors[7] = glm::vec3( _chunkSize.xWidth, 0.0f,  _chunkSize.zWidth);

	createChunks();
}

glm::vec3 World::getVoxelPosition(glm::vec3 worldPos) const
{
    float x = floor(worldPos.x);
    float y = floor(worldPos.y);
    float z = floor(worldPos.z);

    float chunkX = floor(x / _chunkSize.xWidth);
    float chunkZ = floor(z / _chunkSize.zWidth);

    x -= chunkX * _chunkSize.xWidth;
    z -= chunkZ * _chunkSize.zWidth;

    x -= _chunkSize.xWidth / 2;
    z -= _chunkSize.zWidth / 2;

    return glm::vec3(x, y, z);
}

void World::createChunks()
{
    glm::vec3 position{ 0.0f, 0.0f, 0.0f };
    createChunkRec(position, 0);
}

void World::createVoxel(Chunk& chunk, glm::vec3 position, Mesh& chunkMesh, int& vertexCount)
{
    for (int face = 0; face < 6; face++)
    {
        if (!hasSolidVoxel(chunk, position + voxelNeighbors[face]))
        {
            BlockType blockType{ _worldGen.getBlockType(chunk.getBlockByte(position)) };
            std::vector<float> textureCoordinates{ chunk.getTextureCoordinates(blockType.getBlockSides(), face)};
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

bool World::hasSolidVoxel(Chunk& chunk, glm::vec3 position)
{
    if (chunk.isOutsideChunk(position))
    {
        return hasSolidVoxel(chunk.getPosition() + position);
    }

    return _worldGen.getBlockType(chunk.getBlockByte(position)).isSolid();
}

bool World::hasSolidVoxel(const glm::vec3& worldPos)
{
    if (_worldSize > 0)
    {
        //# of chunks per side depending on the world size
        int dim = _worldSize + _worldSize + 1;

        int boundX = (dim * _chunkSize.xWidth) / 2;
        int boundY = _chunkSize.height - 1;
        int boundZ = (dim * _chunkSize.zWidth) / 2;

        int x = static_cast<int>(floor(worldPos.x));
        int y = static_cast<int>(floor(worldPos.y));
        int z = static_cast<int>(floor(worldPos.z));

        if (y < 0 || y > boundY ||
            x < -boundX || x > boundX - 1 ||
            z < -boundZ || z > boundZ - 1)
        {
            return false;
        }

        glm::vec3 voxelPos{ getVoxelPosition(worldPos) };
        std::array<float, 3> chunkPos{ floor((float)x / _chunkSize.xWidth), 0.0f, floor((float)z / _chunkSize.zWidth) };
        auto keyIter = _activeChunks.find(chunkPos);
        if (keyIter != _activeChunks.end())
        {
            return _worldGen.getBlockType(keyIter->second.getBlockByte(voxelPos)).isSolid();
        }
        else
        {
            return _worldGen.getBlockType(_worldGen.getVoxel(worldPos)).isSolid();
        }
    }
    else
    {
        return false;
    }

    return false;
}

void World::update()
{
    checkCurrentChunk();
}

void World::draw(ShaderProgram shader)
{
    for (auto& chunk : _activeChunks)
    {
        chunk.second.draw(shader);
    }
}

void World::createChunk(glm::vec3 position)
{
    std::array<float, 3> positionArray{ position.x, position.y, position.z };

    if (_activeChunks.find(positionArray) == _activeChunks.end())
    {
        glm::vec3 chunkPos{ position.x, position.y, position.z };
        _activeChunks[positionArray] = Chunk(chunkPos, _chunkSize);
        _activeChunks[positionArray].populateBlockMap(_worldGen);

        Mesh chunkMesh;
        int vertexCount = 0;

        for (int y = 0; y < _chunkSize.height; y++)
        {
            for (int x = -_chunkSize.xWidth / 2; x < _chunkSize.xWidth / 2; x++)
            {
                for (int z = -_chunkSize.zWidth / 2; z < _chunkSize.zWidth / 2; z++)
                {
                    glm::vec3 voxelPosition{ x, y, z };
                    if (_worldGen.getBlockType(_activeChunks[positionArray].getBlockByte(voxelPosition)).isSolid())
                    {
                        createVoxel(_activeChunks[positionArray], voxelPosition, chunkMesh, vertexCount);
                    }
                }
            }
        }

        _activeChunks[positionArray].setChunkMesh(chunkMesh);
    }
}

void World::createChunkRec(glm::vec3 position, int recDepth)
{
    createChunk(position);

    if (recDepth < _worldSize)
    {
        for (auto nPos : _chunkNeighbors)
        {
            createChunkRec(position + nPos, recDepth + 1);
        }
    }
}

void World::checkCurrentChunk()
{
    glm::vec3 cameraPos = _camera->getPosition();

    float x = floor(cameraPos.x / _chunkSize.xWidth);
    float y = 0.0f;
    float z = floor(cameraPos.z / _chunkSize.zWidth);

    std::array<float, 3> chunkPos{ x, y, z };

    if (_currentChunk[0] != chunkPos[0] ||
        _currentChunk[1] != chunkPos[1] ||
        _currentChunk[2] != chunkPos[2])
    {
        std::cout << chunkPos[0] << " " << chunkPos[1] << " " << chunkPos[2] << std::endl;
        _currentChunk = chunkPos;
    }
}

