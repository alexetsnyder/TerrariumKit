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
    _terrainGen.init(_world->getChunkSize(), 32, 16);

    //createChunks();
    queueChunks();
}

void ChunkManager::queueChunks()
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
    int viewDistanceInChunks = _world->getWorldSize();
    float startX = currentChunkId.getX() - viewDistanceInChunks;
    float endX = currentChunkId.getX() + viewDistanceInChunks;
    float startZ = currentChunkId.getZ() - viewDistanceInChunks;
    float endZ = currentChunkId.getZ() + viewDistanceInChunks;

    for (float x = startX; x < endX + 1; x++)
    {
        for (float z = startZ; z < endZ + 1; z++)
        {
            ChunkID chunkId{ _world->getChunkSize(), x, z };
            auto mapIter = _inactiveChunkMap.find(chunkId.getID());
            if (mapIter != _inactiveChunkMap.end())
            {
                _activeChunkMap[mapIter->first] = mapIter->second;
                _inactiveChunkMap.erase(mapIter);
            }
            else
            {
                _activeChunkMap[chunkId.getID()] = Chunk{ chunkId.getPosition(), _world->getChunkSize() };
                _chunkIdQueue.push(chunkId);
            }
        }
    }
}

void ChunkManager::createChunks(int n)
{
    int count = 0;
    while (!_chunkIdQueue.empty() && count++ < n)
    {
        createChunk();
    }
}

void ChunkManager::update()
{
    if (!_chunkIdQueue.empty())
    {
        createChunks(1);
    }

    if (_world->isInfinite() && _world->hasCurrentChunkIdChanged())
    {
        //createChunks();
        queueChunks();
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

    return _terrainGen.getBlockType(chunk.getBlockByte(position)).isSolid();
}

bool ChunkManager::hasSolidVoxel(const glm::vec3& worldPos) const
{
    //isOutsideWorld can only return true if no infinite terrain generation
    if (_world->isOutsideWorld(worldPos))
    {
        return false;
    }

    if (worldPos.y < 0 || worldPos.y > _world->getChunkSize().height - 1)
    {
        return false;
    }

    ChunkID chunkId{ _world->getChunkSize(), worldPos};
    glm::vec3 voxelPos{ chunkId.getRelativeVoxelPosition(worldPos) };

    const auto mapIter = _activeChunkMap.find(chunkId.getID());
    if (mapIter != _activeChunkMap.end() && mapIter->second.hasPopulatedBlockMap())
    {
        return _terrainGen.getBlockType(mapIter->second.getBlockByte(voxelPos)).isSolid();
    }
    else
    {
        return _terrainGen.getBlockType(_terrainGen.getVoxel(worldPos)).isSolid();
    }
}

void ChunkManager::createChunk()
{
    if (!_chunkIdQueue.empty())
    {
        ChunkID chunkId{ _chunkIdQueue.front() };
        _chunkIdQueue.pop();

        _activeChunkMap[chunkId.getID()].populateBlockMap(_terrainGen);

        Mesh chunkMesh;
        int vertexCount = 0;
        ChunkSize chunkSize{ _world->getChunkSize() };

        for (int y = 0; y < chunkSize.height; y++)
        {
            for (int x = 0; x < chunkSize.xWidth; x++)
            {
                for (int z = 0; z < chunkSize.zWidth; z++)
                {
                    glm::vec3 voxelPosition{ x, y, z };
                    if (_terrainGen.getBlockType(_activeChunkMap[chunkId.getID()].getBlockByte(voxelPosition)).isSolid())
                    {
                        createVoxel(_activeChunkMap[chunkId.getID()], voxelPosition, chunkMesh, vertexCount);
                    }
                }
            }
        }

        _activeChunkMap[chunkId.getID()].setChunkMesh(chunkMesh);
    }
}

void ChunkManager::createVoxel(const Chunk& chunk, const glm::vec3& voxelPosition, Mesh& chunkMesh, int& vertexCount)
{
    for (int face = 0; face < 6; face++)
    {
        if (!hasSolidVoxel(chunk, voxelPosition + voxelNeighbors[face]))
        {
            BlockType blockType{ _terrainGen.getBlockType(chunk.getBlockByte(voxelPosition)) };
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
