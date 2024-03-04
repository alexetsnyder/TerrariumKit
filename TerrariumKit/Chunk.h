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
			Chunk(const IChunkMediator* chunkMediator, 
				  const ITerrainGen* terrainGen, 
				  CompTK::IMeshRenderer* meshRenderer, 
				  glm::vec3 position, 
				  ChunkSize chunkSize);
			~Chunk();
			Chunk(const Chunk&) = delete;

			bool hasPopulatedVoxelMap() const;
			bool isOutsideChunk(const glm::vec3& position) const;
			GLubyte getVoxelByte(const glm::vec3& position) const;
			
			void populateVoxelMap();
			void createChunkMesh(Mesh& chunkMesh);
			void sendChunkMesh(Mesh& chunkMesh);
		
			void setMeshRenderer(CompTK::IMeshRenderer* meshRenderer);
			void draw(const ShaderProgram& shader) const;

		private:
			void create(Mesh& mesh);
			std::vector<float> getTextureCoordinates(VoxelSides voxelSides, int face) const;
			void createVoxel(const glm::vec3& voxelPosition, Mesh& chunkMesh, int& vertexCount);
			bool hasSolidVoxel(const glm::vec3& position) const;
			std::string getFaceName(VoxelSides voxelSides, int face) const;
			int convertPositionToIndex(const glm::vec3& position) const;

			std::vector<GLubyte> voxels_;
			bool hasPopulatedVoxelMap_;

			const IChunkMediator* chunkMediator_;
			const ITerrainGen* terrainGen_;
			CompTK::IMeshRenderer* meshRenderer_;
			glm::vec3 position_;
			ChunkSize size_;
			TextureAtlas atlas_;
	};
}
