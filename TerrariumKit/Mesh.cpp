#include "Mesh.h"

Mesh::Mesh()
{

}

std::vector<Vertex>& Mesh::getVertices()
{
	return vertices_;
}

std::vector<int>& Mesh::getIndices()
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
