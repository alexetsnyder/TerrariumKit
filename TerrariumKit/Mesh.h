#pragma once

#include <vector>

namespace RenderTK
{
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

            const bool empty();
            const std::vector<Vertex>& getVertices() const;
            const std::vector<int>& getIndices() const;

            void addVertex(Vertex vertex);
            void addIndex(int index);

        private:
            std::vector<Vertex> vertices_;
            std::vector<int> indices_;
    };
}
