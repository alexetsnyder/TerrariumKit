#pragma once

#include "Mesh.h"
#include "TextureAtlas.h"
#include "Texture.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

class Chunk
{
	public:
		Chunk();

		void init(int xWidth, int zWidth, int height);

		glm::mat4 getModelMatrix();

		void draw();

	private:
		void createTextureAtlas();
		Mesh getChunkMesh();
		void createVoxel(glm::vec3 position, Mesh& chunkMesh, int& vertexCount);
		void setChunkMesh(Mesh& chunkMesh);

		void genAll();
		void bindAll();
		void unbindAll();

		int _xWidth;
		int _zWidth;
		int _height;

		glm::vec3 _position;

		TextureAtlas _atlas;
		Texture _texture;

		GLuint _vao;
		GLuint _vbo;
		GLuint _ebo;

		GLuint _indicesCount;
};

