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

        void addVertex(Vertex vertex);
        void addIndex(int index);

    private:
        std::vector<Vertex> vertices_;
        std::vector<int> indices_;
};

