#pragma once

#include "Chunk.h"
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

		glm::vec3 getVoxelPosition(glm::vec3 worldPos) const;
		std::array<float, 3> getChunkPosition(glm::vec3 worldPos) const;

		void createChunks();
		void createVoxel(Chunk& chunk, glm::vec3 position, Mesh& chunkMesh, int& vertexCount);

		bool hasSolidVoxel(const Chunk& chunk, const glm::vec3& position) const;
		bool hasSolidVoxel(const glm::vec3& worldPos) const;

		void update();
		void draw(ShaderProgram shader);

	private:
		void createChunk(glm::vec3 position);
		void createChunkRec(glm::vec3 position, int recDepth);
		void checkCurrentChunk();

		int _worldSize;
		ChunkSize _chunkSize;
		WorldGen _worldGen;
		std::map<std::array<float, 3>, Chunk> _activeChunks;
		glm::vec3 _chunkNeighbors[8];
		const Camera* _camera;
		std::array<float, 3> _currentChunk;
};

