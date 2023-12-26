#include "World.h"

#include "Mesh.h"
#include "Chunk.h"
#include "ChunkID.h"

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
    : _chunkSize{}, _currentChunk{}
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
    _currentChunk.init(_chunkSize, 0.0f, 0.0f);

	createChunks();
}

void World::createChunks()
{
    if (!_activeChunks.empty())
    {
        for (const auto& pair : _activeChunks)
        {
            _inactiveChunks[pair.first] = pair.second;
        }

        for (const auto& pair : _inactiveChunks)
        {
            _activeChunks.erase(pair.first);
        }
    }

    int viewDistanceInChunks = abs(_worldSize);
    float startX = _currentChunk.getX() - viewDistanceInChunks;
    float endX = _currentChunk.getX() + viewDistanceInChunks;
    float startZ = _currentChunk.getZ() - viewDistanceInChunks;
    float endZ = _currentChunk.getZ() + viewDistanceInChunks;

    for (float x = startX; x < endX + 1; x++)
    {
        for (float z = startZ; z < endZ + 1; z++)
        {
            ChunkID chunkId{ _chunkSize, x, z };
            auto mapIter = _inactiveChunks.find(chunkId.getID());
            if (mapIter != _inactiveChunks.end())
            {
                _activeChunks[mapIter->first] = mapIter->second;
                _inactiveChunks.erase(mapIter);
            }
            else
            {
                createChunk(chunkId);
            }
        }
    }
}

void World::createVoxel(const Chunk& chunk, const glm::vec3& voxelPosition, Mesh& chunkMesh, int& vertexCount)
{
    for (int face = 0; face < 6; face++)
    {
        if (!hasSolidVoxel(chunk, voxelPosition + voxelNeighbors[face]))
        {
            BlockType blockType{ _worldGen.getBlockType(chunk.getBlockByte(voxelPosition)) };
            std::vector<float> textureCoordinates{ chunk.getTextureCoordinates(blockType.getBlockSides(), face) };
            for (int vertex = 0; vertex < 4; vertex++)
            {
                Vertex newVertex{};

                //3 components in 1 vertex, and 4 vertex in a face: 3 * 4 = 12
                newVertex.position.x = voxelPosition.x + voxelVertices[12 * face + 3 * vertex] + 0.5f;
                newVertex.position.y = voxelPosition.y + voxelVertices[12 * face + 3 * vertex + 1] + 0.5f;
                newVertex.position.z = voxelPosition.z + voxelVertices[12 * face + 3 * vertex + 2] + 0.5f;

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

bool World::hasSolidVoxel(const Chunk& chunk, const glm::vec3& position) const
{
    if (chunk.isOutsideChunk(position))
    {
        return hasSolidVoxel(chunk.getPosition() + position);
    }

    return _worldGen.getBlockType(chunk.getBlockByte(position)).isSolid();
}

bool World::hasSolidVoxel(const glm::vec3& worldPos) const
{
    if (_worldSize > 0)
    {
        //# of chunks per side depending on the world size
        int dim = _worldSize + _worldSize + 1;

        int boundY = _chunkSize.height - 1;

        int lowerBoundX = -(dim * _chunkSize.xWidth) / 2 + _chunkSize.xWidth / 2;
        int lowerBoundZ = -(dim * _chunkSize.zWidth) / 2 + _chunkSize.zWidth / 2;
        int upperBoundX =  (dim * _chunkSize.xWidth) / 2 + _chunkSize.xWidth / 2;    
        int upperBoundZ =  (dim * _chunkSize.zWidth) / 2 + _chunkSize.zWidth / 2;

        int x = static_cast<int>(floor(worldPos.x));
        int y = static_cast<int>(floor(worldPos.y));
        int z = static_cast<int>(floor(worldPos.z));

        if (y < 0 || y > boundY ||
            x < lowerBoundX || x > upperBoundX - 1 ||
            z < lowerBoundZ || z > upperBoundZ - 1)
        {
            return false;
        }      
    }

    if (worldPos.y < 0)
    {
        return false;
    }

    ChunkID chunkId{ _chunkSize, worldPos };
    glm::vec3 voxelPos{ chunkId.getRelativeVoxelPosition(worldPos) };

    auto mapIter = _activeChunks.find(chunkId.getID());
    if (mapIter != _activeChunks.end())
    {
        return _worldGen.getBlockType(mapIter->second.getBlockByte(voxelPos)).isSolid();
    }
    else
    {
        return _worldGen.getBlockType(_worldGen.getVoxel(worldPos)).isSolid();
    } 
}

void World::update()
{
    checkCurrentChunk();
}

void World::draw(const ShaderProgram& shader) const 
{
    for (auto& chunk : _activeChunks)
    {
        chunk.second.draw(shader);
    }
}

void World::createChunk(ChunkID chunkId)
{
    _activeChunks[chunkId.getID()] = Chunk{ chunkId.getPosition(), _chunkSize };
    _activeChunks[chunkId.getID()].populateBlockMap(_worldGen);

    Mesh chunkMesh;
    int vertexCount = 0;

    for (int y = 0; y < _chunkSize.height; y++)
    {
        for (int x = 0; x < _chunkSize.xWidth; x++)
        {
            for (int z = 0; z < _chunkSize.zWidth; z++)
            {
                glm::vec3 voxelPosition{ x, y, z };
                if (_worldGen.getBlockType(_activeChunks[chunkId.getID()].getBlockByte(voxelPosition)).isSolid())
                {
                    createVoxel(_activeChunks[chunkId.getID()], voxelPosition, chunkMesh, vertexCount);
                }
            }
        }
    }

    _activeChunks[chunkId.getID()].setChunkMesh(chunkMesh);
}

void World::checkCurrentChunk()
{
    glm::vec3 cameraPos = _camera->getPosition();

    ChunkID chunkId{ _chunkSize, cameraPos };

    if (!chunkId.Equals(_currentChunk))
    {
        std::cout <<"X: " << chunkId.getX() << " Z: " << chunkId.getZ() << std::endl;
        _currentChunk = chunkId;

        if (_worldSize < 0)
        {
            createChunks();
        }  
    }
}

