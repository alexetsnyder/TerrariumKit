#pragma once

#include "Chunk.h"
#include "ChunkID.h"
#include "ChunkPool.h"
#include "IChunkMediator.h"
#include "IMeshRenderer.h"
#include "ITerrainGen.h"
#include "ShaderProgram.h"
#include "World.h"

#include <glm/glm.hpp>

#include <array>
#include <map>
#include <queue>

namespace ProcGenTK
{
	struct ChunkMeshInfo
	{
		Chunk* chunkPointer;
		RenderTK::Mesh chunkMesh;
	};

	class ChunkManager : public IChunkMediator
	{
		public:
			ChunkManager(const World* world);
			~ChunkManager();

			bool hasSolidVoxel(const glm::vec3& worldPos) const override;

			void queueChunks();
			void createChunks(int n);
			void sendChunkData(int n);

			void update();
			void draw(const RenderTK::ShaderProgram& program);

		private:
			ChunkMeshInfo nextChunkMeshInfo();
			void createChunk(Chunk* chunk);
			void setAllInactive();

			//const CompTK::IMeshRenderer* nullRenderer_;
			ChunkPool pool_;

			const World* world_;
			const ITerrainGen* terrainGen_;
			std::queue<Chunk*> chunkCreateQueue_;

			std::queue<ChunkMeshInfo> chunkMeshInfoQueue_;

			std::map<std::array<float, 3>, Chunk*> activeChunkMap_;
			std::map<std::array<float, 3>, Chunk*> inactiveChunkMap_;
	};
}
