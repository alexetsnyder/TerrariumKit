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
		Chunk(int width, int height);

		void init();

		glm::mat4 getModelMatrix();

		void draw();

	private:
		void createTextureAtlas();
		Mesh getChunkMesh();
		void setChunkMesh(Mesh& chunkMesh);

		void genAll();
		void bindAll();
		void unbindAll();

		int _width;
		int _height;

		glm::vec3 _position;

		TextureAtlas _atlas;
		Texture _texture;

		GLuint _vao;
		GLuint _vbo;
		GLuint _ebo;

		GLuint _indicesCount;
};

