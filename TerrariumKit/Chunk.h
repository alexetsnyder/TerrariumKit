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
			bool hasPopulatedVoxelMap() const;

			GLubyte getVoxelByte(const glm::vec3& position) const;
			std::vector<float> getTextureCoordinates(VoxelSides voxelSides, int face) const;

			void setNoDraw(bool noDraw);

			void populateVoxelMap();

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
			std::string getFaceName(VoxelSides voxelSides, int face) const;
			int convertPositionToIndex(const glm::vec3& position) const;

			std::vector<GLubyte> voxels_;
			bool hasPopulatedVoxelMap_;

			const IChunkMediator* chunkMediator_;
			const ITerrainGen* terrainGen_;
			ChunkSize size_;
			glm::vec3 position_;

			TextureAtlas atlas_;
			Texture texture_;

			GLuint vao_;
			GLuint vbo_;
			GLuint ebo_;

			GLuint indicesCount_;

			bool noDraw_;
	};
}
