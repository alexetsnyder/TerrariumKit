#include "Chunk.h"

#include "ErrorLog.h"
#include "Mesh.h"
#include "VoxelType.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

namespace ProcGenTK
{
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

    std::vector<std::string> voxelNames =
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

    Chunk::Chunk(const IChunkMediator* chunkMediator, 
                 const ITerrainGen* terrainGen, 
                 CompTK::IMeshRenderer* meshRenderer, 
                 glm::vec3 position, ChunkSize chunkSize)
        : chunkMediator_{ chunkMediator}, terrainGen_{ terrainGen }, meshRenderer_{ meshRenderer },
          position_{ position}, size_{ chunkSize }, atlas_{ 256, 16, voxelNames }
    {
        hasPopulatedVoxelMap_ = false;
        voxels_.resize(chunkSize.xWidth * chunkSize.zWidth * chunkSize.height);
    }

    Chunk::~Chunk()
    {
        delete meshRenderer_;
    }

    bool Chunk::hasPopulatedVoxelMap() const
    {
        return hasPopulatedVoxelMap_;
    }

    bool Chunk::isOutsideChunk(const glm::vec3& position) const
    {
        int xBound = size_.xWidth - 1;
        int yBound = size_.height - 1;
        int zBound = size_.zWidth - 1;

        int x = static_cast<int>(floor(position.x));
        int y = static_cast<int>(floor(position.y));
        int z = static_cast<int>(floor(position.z));

        if (x < 0 || x > xBound ||
            z < 0 || z > zBound ||
            y < 0 || y > yBound)
        {
            return true;
        }

        return false;
    }

    GLubyte Chunk::getVoxelByte(const glm::vec3& position) const
    {
        int index = convertPositionToIndex(position);
        return voxels_[index];
    }

    void Chunk::populateVoxelMap()
    {
        for (int y = 0; y < size_.height; y++)
        {
            for (int x = 0; x < size_.xWidth; x++)
            {
                for (int z = 0; z < size_.zWidth; z++)
                {
                    glm::vec3 voxelPosition{ x, y, z };
                    int index = convertPositionToIndex(voxelPosition);
                    voxels_[index] = terrainGen_->getVoxel(position_ + voxelPosition);
                }
            }
        }

        hasPopulatedVoxelMap_ = true;
    }

    void Chunk::createChunkMesh(Mesh& chunkMesh)
    {
        int vertexCount = 0;
        for (int y = 0; y < size_.height; y++)
        {
            for (int x = 0; x < size_.xWidth; x++)
            {
                for (int z = 0; z < size_.zWidth; z++)
                {
                    glm::vec3 voxelPosition{ x, y, z };
                    if (terrainGen_->getVoxelType(getVoxelByte(voxelPosition)).isSolid())
                    {
                        createVoxel(voxelPosition, chunkMesh, vertexCount);
                    }
                }
            }
        }
    }

    void Chunk::sendChunkMesh(Mesh& chunkMesh)
    {
        meshRenderer_->sendData(chunkMesh);
    }

    void Chunk::setMeshRenderer(CompTK::IMeshRenderer* meshRenderer)
    {
        delete meshRenderer_;
        meshRenderer_ = meshRenderer;
    }

    void Chunk::draw(const ShaderProgram& shader) const
    {
        meshRenderer_->draw(shader, position_);
    }

    std::vector<float> Chunk::getTextureCoordinates(VoxelSides voxelSides, int face) const
    {
        return atlas_.getTextureCoordinates(getFaceName(voxelSides, face));
    }

    void Chunk::createVoxel(const glm::vec3& voxelPosition, Mesh& chunkMesh, int& vertexCount)
    {
        for (int face = 0; face < 6; face++)
        {
            if (!hasSolidVoxel(voxelPosition + voxelNeighbors[face]))
            {
                VoxelType voxelType{ terrainGen_->getVoxelType(getVoxelByte(voxelPosition)) };
                std::vector<float> textureCoordinates{ getTextureCoordinates(voxelType.getVoxelSides(), face) };
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

    bool Chunk::hasSolidVoxel(const glm::vec3& position) const
    {
        if (isOutsideChunk(position))
        {
            return chunkMediator_->hasSolidVoxel(position_ + position);
        }

        return terrainGen_->getVoxelType(getVoxelByte(position)).isSolid();
    }

    std::string Chunk::getFaceName(VoxelSides voxelSides, int face) const
    {
        std::string faceName{};

        switch (face)
        {
            case 0:
                faceName = voxelSides.frontTextureName;
                break;
            case 1:
                faceName = voxelSides.backTextureName;
                break;
            case 2:
                faceName = voxelSides.leftTextureName;
                break;
            case 3:
                faceName = voxelSides.rightTextureName;
                break;
            case 4:
                faceName = voxelSides.topTextureName;
                break;
            case 5:
                faceName = voxelSides.bottomTextureName;
                break;
        }

        return faceName;
    }

    int Chunk::convertPositionToIndex(const glm::vec3& position) const
    {
        int x = static_cast<int>(floor(position.x));
        int y = static_cast<int>(floor(position.y));
        int z = static_cast<int>(floor(position.z));

        int index = y * size_.xWidth * size_.zWidth + x * size_.zWidth + z;

        if (index < 0 || index >= voxels_.size())
        {
            std::string errorMsg{ "" };
            std::size_t blocksSize = voxels_.size();
            if (blocksSize == 0)
            {
                errorMsg += "Blocks vector has size zero";
            }
            else
            {
                errorMsg += "Index " + index;
                errorMsg += "is out of range 0 to " + blocksSize;
            }
            logError("Chunk::convertPositionToIndex", errorMsg);
        }

        return index;
    }
}
