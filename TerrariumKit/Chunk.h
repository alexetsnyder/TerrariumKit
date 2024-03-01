#pragma once

#include "IChunkMediator.h"
#include "ITerrainGen.h"
#include "Mesh.h"
#include "IMeshRenderer.h"
#include "ShaderProgram.h"
#include "Structs.h"
#include "TextureAtlas.h"
#include "VoxelType.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

namespace ProcGenTK
{
	class Chunk
	{
		public:
			Chunk(const IChunkMediator* chunkMediator, const ITerrainGen* terrainGen, glm::vec3 position, ChunkSize chunkSize);
			~Chunk();
			Chunk(const Chunk&) = delete;

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
			void free();
			std::string getFaceName(VoxelSides voxelSides, int face) const;
			int convertPositionToIndex(const glm::vec3& position) const;

			std::vector<GLubyte> voxels_;
			bool hasPopulatedVoxelMap_;

			const IChunkMediator* chunkMediator_;
			const ITerrainGen* terrainGen_;
			ChunkSize size_;
			glm::vec3 position_;

			bool noDraw_;
			TextureAtlas atlas_;
			CompTK::IMeshRenderer* meshRenderer_;
	};
}
