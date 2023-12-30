#pragma once

#include "Mesh.h"
#include "TextureAtlas.h"
#include "Texture.h"
#include "BlockType.h"
#include "Structs.h"
#include "TerrainGen.h"

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
		glm::vec3 getPosition() const;
		bool hasPopulatedBlockMap() const;

		GLubyte getBlockByte(const glm::vec3& position) const;
		std::vector<float> getTextureCoordinates(BlockSides blockSides, int face) const;

		void setNoDraw(bool noDraw);

		void init(glm::vec3 position, ChunkSize chunkSize);

		void populateBlockMap(TerrainGen worldGen);

		void setChunkMesh(Mesh& chunkMesh);
		
		bool isOutsideChunk(glm::vec3 position) const;

		void draw(const ShaderProgram& shader) const;

		void deleteAll();

	private:
		void createTextureAtlas();
		void genAll();	
		void bindAll();
		void unbindAll();
		std::string getFaceName(BlockSides blockSides, int face) const;
		int convertPositionToIndex(const glm::vec3& position) const;

		std::vector<GLubyte> _blocks;
		bool _hasPopulatedBlockMap;

		ChunkSize _size;
		glm::vec3 _position;

		TextureAtlas _atlas;
		Texture _texture;

		GLuint _vao;
		GLuint _vbo;
		GLuint _ebo;

		GLuint _indicesCount;

		bool _noDraw;
};

