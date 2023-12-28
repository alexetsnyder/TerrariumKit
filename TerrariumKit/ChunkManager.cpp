#include "ChunkManager.h"

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

ChunkManager::ChunkManager()
{
	_world = nullptr;
}

ChunkManager::ChunkManager(const World& world)
{
	init(world);
}

void ChunkManager::init(const World& world)
{
	_world = &world;
    _worldGen.init(_world->getChunkSize(), 32, 16);

    createChunks();
}

void ChunkManager::queueChunks()
{
   
}

void ChunkManager::createChunks()
{
    if (!_activeChunkMap.empty())
    {
        for (const auto& pair : _activeChunkMap)
        {
            _inactiveChunkMap[pair.first] = pair.second;
        }

        for (const auto& pair : _inactiveChunkMap)
        {
            _activeChunkMap.erase(pair.first);
        }
    }

    ChunkID currentChunkId = _world->getCurrentChunkID();
    int viewDistanceInChunks = abs(_world->getWorldSize());
    float startX = currentChunkId.getX() - viewDistanceInChunks;
    float endX = currentChunkId.getX() + viewDistanceInChunks;
    float startZ = currentChunkId.getZ() - viewDistanceInChunks;
    float endZ = currentChunkId.getZ() + viewDistanceInChunks;

    for (float x = startX; x < endX + 1; x++)
    {
        for (float z = startZ; z < endZ + 1; z++)
        {
            ChunkID chunkId{ _world->getChunkSize(), x, z};
            auto mapIter = _inactiveChunkMap.find(chunkId.getID());
            if (mapIter != _inactiveChunkMap.end())
            {
                _activeChunkMap[mapIter->first] = mapIter->second;
                _inactiveChunkMap.erase(mapIter);
            }
            else
            {
                createChunk(chunkId);
            }
        }
    }
}

void ChunkManager::update()
{
    if (_world->getWorldSize() < 0 && _world->hasCurrentChunkIdChanged())
    {
        createChunks();
    }
}

void ChunkManager::draw(const ShaderProgram& program)
{
    for (auto& chunk : _activeChunkMap)
    {
        chunk.second.draw(program);
    }
}

bool ChunkManager::hasSolidVoxel(const Chunk& chunk, const glm::vec3& position) const
{
    if (chunk.isOutsideChunk(position))
    {
        return hasSolidVoxel(chunk.getPosition() + position);
    }

    return _worldGen.getBlockType(chunk.getBlockByte(position)).isSolid();
}

bool ChunkManager::hasSolidVoxel(const glm::vec3& worldPos) const
{
    if (_world->isOutsideWorld(worldPos))
    {
        return false;
    }

    //isOutsideWorld only return true if no infinite terrain generation
    if (worldPos.y < 0)
    {
        return false;
    }

    ChunkID chunkId{ _world->getChunkSize(), worldPos};
    glm::vec3 voxelPos{ chunkId.getRelativeVoxelPosition(worldPos) };

    auto mapIter = _activeChunkMap.find(chunkId.getID());
    if (mapIter != _activeChunkMap.end())
    {
        return _worldGen.getBlockType(mapIter->second.getBlockByte(voxelPos)).isSolid();
    }
    else
    {
        return _worldGen.getBlockType(_worldGen.getVoxel(worldPos)).isSolid();
    }
}

void ChunkManager::createChunk(ChunkID chunkId)
{
    ChunkSize chunkSize{ _world->getChunkSize() };
    _activeChunkMap[chunkId.getID()] = Chunk{ chunkId.getPosition(), chunkSize };
    _activeChunkMap[chunkId.getID()].populateBlockMap(_worldGen);

    Mesh chunkMesh;
    int vertexCount = 0;

    for (int y = 0; y < chunkSize.height; y++)
    {
        for (int x = 0; x < chunkSize.xWidth; x++)
        {
            for (int z = 0; z < chunkSize.zWidth; z++)
            {
                glm::vec3 voxelPosition{ x, y, z };
                if (_worldGen.getBlockType(_activeChunkMap[chunkId.getID()].getBlockByte(voxelPosition)).isSolid())
                {
                    createVoxel(_activeChunkMap[chunkId.getID()], voxelPosition, chunkMesh, vertexCount);
                }
            }
        }
    }

    _activeChunkMap[chunkId.getID()].setChunkMesh(chunkMesh);
}

void ChunkManager::createVoxel(const Chunk& chunk, const glm::vec3& voxelPosition, Mesh& chunkMesh, int& vertexCount)
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
