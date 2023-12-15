#include "Mesh.h"

Mesh::Mesh()
{

}

std::vector<Vertex>& Mesh::getVertices()
{
	return _vertices;
}

std::vector<int>& Mesh::getIndices()
{
	return _indices;
}

void Mesh::setVertices(const std::vector<Vertex>& vertices)
{
	_vertices = vertices;
}

void Mesh::setIndices(const std::vector<int>& indices)
{
	_indices = indices;
}

void Mesh::addVertex(Vertex vertex)
{
	_vertices.push_back(vertex);
}

void Mesh::addIndex(int index)
{
	_indices.push_back(index);
}

