#pragma once

#include "VoxelType.h"
#include "IChunkMediator.h"
#include "ITerrainGen.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Structs.h"
#include "TextureAtlas.h"
#include "Texture.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>

namespace ProcGenTK
{
	class Chunk
	{
		public:
			Chunk(const IChunkMediator* chunkMediator, const ITerrainGen* terrainGen, glm::vec3 position, ChunkSize chunkSize);
			~Chunk();
			Chunk(const Chunk&) = delete;
			Chunk& operator=(const Chunk&) = delete;

			glm::mat4 modelMatrix() const;
			bool hasPopulatedBlockMap() const;

			GLubyte getVoxelByte(const glm::vec3& position) const;
			std::vector<float> getTextureCoordinates(BlockSides blockSides, int face) const;

			void setNoDraw(bool noDraw);

			void populateBlockMap();

			void createChunkMesh(Mesh& chunkMesh);

			void setChunkMesh(Mesh& chunkMesh);

			bool isOutsideChunk(glm::vec3 position) const;

			void draw(const ShaderProgram& shader) const;

		private:
			void createVoxel(const glm::vec3& voxelPosition, Mesh& chunkMesh, int& vertexCount);
			bool hasSolidVoxel(const glm::vec3& position) const;
			void createTextureAtlas();
			void genAll();
			void bindAll();
			void unbindAll();
			void free();
			std::string getFaceName(BlockSides blockSides, int face) const;
			int convertPositionToIndex(const glm::vec3& position) const;

			std::vector<GLubyte> _blocks;
			bool _hasPopulatedBlockMap;

			const IChunkMediator* _chunkMediator;
			const ITerrainGen* _terrainGen;
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
}
