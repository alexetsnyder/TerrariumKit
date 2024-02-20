#include "TerrainGen.h"

namespace ProcGenTK
{
	TerrainGen::TerrainGen(ChunkSize chunkSize, float minHeight, float varyHeight)
		: chunkSize_{ chunkSize }
	{
		noise_.SetSeed(42);
		noise_.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
		noise_.SetFractalType(FastNoiseLite::FractalType_FBm);

		minHeight_ = minHeight;
		varyHeight_ = varyHeight;

		VoxelSides voxelSides{};
		voxelTypeLookUp_[0] = VoxelType("air", false, voxelSides);

		voxelSides.frontTextureName = "bedrock";
		voxelSides.backTextureName = "bedrock";
		voxelSides.leftTextureName = "bedrock";
		voxelSides.rightTextureName = "bedrock";
		voxelSides.topTextureName = "bedrock";
		voxelSides.bottomTextureName = "bedrock";
		voxelTypeLookUp_[1] = VoxelType("bedrock", true, voxelSides);

		voxelSides.frontTextureName = "stone";
		voxelSides.backTextureName = "stone";
		voxelSides.leftTextureName = "stone";
		voxelSides.rightTextureName = "stone";
		voxelSides.topTextureName = "stone";
		voxelSides.bottomTextureName = "stone";
		voxelTypeLookUp_[2] = VoxelType("stone", true, voxelSides);

		voxelSides.frontTextureName = "dirt";
		voxelSides.backTextureName = "dirt";
		voxelSides.leftTextureName = "dirt";
		voxelSides.rightTextureName = "dirt";
		voxelSides.topTextureName = "dirt";
		voxelSides.bottomTextureName = "dirt";
		voxelTypeLookUp_[3] = VoxelType("dirt", true, voxelSides);

		voxelSides.frontTextureName = "grassSide";
		voxelSides.backTextureName = "grassSide";
		voxelSides.leftTextureName = "grassSide";
		voxelSides.rightTextureName = "grassSide";
		voxelSides.topTextureName = "grassTop";
		voxelSides.bottomTextureName = "dirt";
		voxelTypeLookUp_[4] = VoxelType("grass", true, voxelSides);

		createVoxelByteLookUp();
	}

	GLubyte TerrainGen::getVoxel(const glm::vec3& position) const
	{
		int y = static_cast<int>(floor(position.y));

		if (y == 0)
		{
			return voxelByteLookUp_.at("bedrock");
		}

		/* TERRAIN GENERATION */

		int height = static_cast<int>(floor(noise_.GetNoise(position.x, position.z) * varyHeight_ + minHeight_));

		if (y > height)
		{
			return voxelByteLookUp_.at("air");
		}
		else if (y >= height)
		{
			return voxelByteLookUp_.at("grass");
		}
		else if (y >= height - 6)
		{
			return voxelByteLookUp_.at("dirt");
		}
		else
		{
			return voxelByteLookUp_.at("stone");
		}
	}

	VoxelType TerrainGen::getVoxelType(GLubyte byte) const
	{
		return voxelTypeLookUp_.at(byte);
	}

	void TerrainGen::createVoxelByteLookUp()
	{
		for (const auto& pair : voxelTypeLookUp_)
		{
			voxelByteLookUp_[pair.second.getName()] = pair.first;
		}
	}
}
