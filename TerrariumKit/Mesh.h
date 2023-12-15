#pragma once

#include <vector>

struct Vertex
{
    struct Position
    {
        float x;
        float y;
        float z;
    } position;

    struct TextureCoordinate
    {
        float u;
        float v;
    } textureCoordinate;
};

class Mesh
{
    public:
        Mesh();

        std::vector<Vertex>& getVertices();
        std::vector<int>& getIndices();

        void setVertices(const std::vector<Vertex>& vertices);
        void setIndices(const std::vector<int>& indices);

        void addVertex(Vertex vertex);
        void addIndex(int index);

    private:
        std::vector<Vertex> _vertices;
        std::vector<int> _indices;
};

