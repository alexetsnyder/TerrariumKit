#pragma once

#include "Chunk.h"
#include "ChunkID.h"
#include "Mesh.h"
#include "WorldGen.h"
#include "ShaderProgram.h"
#include "Camera.h"

#include <glm/glm.hpp>

#include <map>
#include <array>

class World
{
	public:
		World();

		void init(const Camera& camera, int worldSize, ChunkSize chunkSize);

		bool hasSolidVoxel(const Chunk& chunk, const glm::vec3& position) const;
		bool hasSolidVoxel(const glm::vec3& worldPos) const;

		void createChunks();
		void createVoxel(const Chunk& chunk, const glm::vec3& voxelPosition, Mesh& chunkMesh, int& vertexCount);

		void update();
		void draw(const ShaderProgram& shader) const;

	private:
		void createChunk(ChunkID chunkId);
		void checkCurrentChunk();

		int _worldSize;
		ChunkSize _chunkSize;
		WorldGen _worldGen;
		std::map<std::array<float, 2>, Chunk> _activeChunks;
		std::map<std::array<float, 2>, Chunk> _inactiveChunks;
		const Camera* _camera;
		ChunkID _currentChunkId;
};

