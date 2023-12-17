#pragma once

#include "Mesh.h"
#include "TextureAtlas.h"
#include "Texture.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

struct ChunkSize
{
	int xWidth;
	int zWidth;
	int height;
};

class Chunk
{
	public:
		Chunk();

		glm::mat4 getModelMatrix();

		void init(ChunkSize chunkSize);

		Mesh getChunkMesh();
		void createVoxel(glm::vec3 position, Mesh& chunkMesh, int& vertexCount);
		void setChunkMesh(Mesh& chunkMesh);

		void draw();

	private:
		void createTextureAtlas();
		void genAll();
		void bindAll();
		void unbindAll();

		std::vector<GLubyte> _blocks;

		ChunkSize _size;
		glm::vec3 _position;

		TextureAtlas _atlas;
		Texture _texture;

		GLuint _vao;
		GLuint _vbo;
		GLuint _ebo;

		GLuint _indicesCount;
};

