#include "Mesh.h"

Mesh::Mesh()
{

}

const std::vector<Vertex>& Mesh::getVertices() const
{
	return vertices_;
}

const std::vector<int>& Mesh::getIndices() const
{
	return indices_;
}

void Mesh::addVertex(Vertex vertex)
{
	vertices_.push_back(vertex);
}

void Mesh::addIndex(int index)
{
	indices_.push_back(index);
}
