#pragma once

#include "Mesh.h"
#include "TextureAtlas.h"
#include "Texture.h"
#include "BlockType.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "ShaderProgram.h"

#include <vector>
#include <string>

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

		void createVoxel(BlockType blockType, glm::vec3 position, Mesh& chunkMesh, int& vertexCount);
		void setChunkMesh(Mesh& chunkMesh);
		
		bool isOutsideChunk(glm::vec3 position);
		bool hasSolidBlock(glm::vec3 position);

		void draw(ShaderProgram shader);

	private:
		void createTextureAtlas();
		void genAll();
		void bindAll();
		void unbindAll();
		std::string getFaceName(BlockSides blockSides, int face);

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

