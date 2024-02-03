#include "Chunk.h"

#include "Mesh.h"
#include "BlockType.h"
#include "ErrorLog.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

namespace ProcGenTK
{
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

    Chunk::Chunk(glm::vec3 position, ChunkSize chunkSize)
        : _atlas{ 256, 16 }, _size{ chunkSize }
    {
        _hasPopulatedBlockMap = false;
        _vao = 0;
        _vbo = 0;
        _ebo = 0;
        _position = position;
        _blocks.resize(chunkSize.xWidth * chunkSize.zWidth * chunkSize.height);
        _indicesCount = 0;
        _noDraw = false;

        createTextureAtlas();
    }

    Chunk::~Chunk()
    {
        free();
    }

    void Chunk::populateBlockMap(TerrainGen terrainGen)
    {
        for (int y = 0; y < _size.height; y++)
        {
            for (int x = 0; x < _size.xWidth; x++)
            {
                for (int z = 0; z < _size.zWidth; z++)
                {
                    glm::vec3 voxelPosition{ x, y, z };
                    int index = convertPositionToIndex(voxelPosition);
                    _blocks[index] = terrainGen.getVoxel(_position + voxelPosition);
                }
            }
        }

        _hasPopulatedBlockMap = true;
    }

    std::vector<float> Chunk::getTextureCoordinates(BlockSides blockSides, int face) const
    {
        return _atlas.getTextureCoordinates(getFaceName(blockSides, face));
    }

    void Chunk::setNoDraw(bool noDraw)
    {
        _noDraw = noDraw;
    }

    void Chunk::setChunkMesh(Mesh& chunkMesh)
    {
        genAll();
        bindAll();

        _indicesCount = static_cast<GLuint>(chunkMesh.getIndices().size());

        glBufferData(GL_ARRAY_BUFFER, chunkMesh.getVertices().size() * sizeof(Vertex), &chunkMesh.getVertices().front(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoordinate));
        glEnableVertexAttribArray(1);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, chunkMesh.getIndices().size() * sizeof(float), &chunkMesh.getIndices().front(), GL_STATIC_DRAW);

        unbindAll();
    }

    bool Chunk::isOutsideChunk(glm::vec3 position) const
    {
        int xBound = _size.xWidth - 1;
        int yBound = _size.height - 1;
        int zBound = _size.zWidth - 1;

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

    void Chunk::draw(const ShaderProgram& shader) const
    {
        if (!_noDraw)
        {
            glm::mat4 model{ getModelMatrix() };

            shader.setUniform("model", model);

            _texture.bind();

            glBindVertexArray(_vao);
            glDrawElements(GL_TRIANGLES, _indicesCount, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
    }

    void Chunk::createTextureAtlas()
    {
        _atlas.createAtlas(blockNames);
        _texture.init("Assets/Textures/Atlas.png");
    }

    void Chunk::genAll()
    {
        glGenVertexArrays(1, &_vao);
        glGenBuffers(1, &_vbo);
        glGenBuffers(1, &_ebo);
    }

    void Chunk::free()
    {
        if (_vao > 0)
        {
            glDeleteVertexArrays(1, &_vao);
        }

        if (_vbo > 0)
        {
            glDeleteBuffers(1, &_vbo);
        }

        if (_ebo > 0)
        {
            glDeleteBuffers(1, &_ebo);
        }
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

    std::string Chunk::getFaceName(BlockSides blockSides, int face) const
    {
        std::string faceName{};

        switch (face)
        {
        case 0:
            faceName = blockSides.frontTextureName;
            break;
        case 1:
            faceName = blockSides.backTextureName;
            break;
        case 2:
            faceName = blockSides.leftTextureName;
            break;
        case 3:
            faceName = blockSides.rightTextureName;
            break;
        case 4:
            faceName = blockSides.topTextureName;
            break;
        case 5:
            faceName = blockSides.bottomTextureName;
            break;
        }

        return faceName;
    }

    glm::mat4 Chunk::getModelMatrix() const
    {
        glm::mat4 model{ 1.0f };
        model = glm::translate(model, _position);
        return model;
    }

    GLubyte Chunk::getBlockByte(const glm::vec3& position) const
    {
        int index = convertPositionToIndex(position);
        return _blocks[index];
    }

    glm::vec3 Chunk::getPosition() const
    {
        return _position;
    }

    bool Chunk::hasPopulatedBlockMap() const
    {
        return _hasPopulatedBlockMap;
    }

    int Chunk::convertPositionToIndex(const glm::vec3& position) const
    {
        int x = static_cast<int>(floor(position.x));
        int y = static_cast<int>(floor(position.y));
        int z = static_cast<int>(floor(position.z));

        int index = y * _size.xWidth * _size.zWidth + x * _size.zWidth + z;

        if (index < 0 || index >= _blocks.size())
        {
            std::string errorMsg{ "" };
            std::size_t blocksSize = _blocks.size();
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


