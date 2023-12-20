#pragma once

#include "Mesh.h"
#include "TextureAtlas.h"
#include "Texture.h"
#include "BlockType.h"
#include "Structs.h"
#include "WorldGen.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "ShaderProgram.h"

#include <vector>
#include <string>

class Chunk
{
	public:
		Chunk();
		Chunk(glm::vec3 position, ChunkSize chunkSize);

		glm::mat4 getModelMatrix() const;
		GLubyte getBlockByte(glm::vec3 position);
		glm::vec3 getPosition() const;

		int convertPositionToIndex(glm::vec3 position) const;

		void init(glm::vec3 position, ChunkSize chunkSize);

		void populateBlockMap(WorldGen worldGen);

		std::vector<float> getTextureCoordinates(BlockSides blockSides, int face);

		void setChunkMesh(Mesh& chunkMesh);
		
		bool isOutsideChunk(glm::vec3 position) const;

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

