#include "TerrainGen.h"

namespace ProcGenTK
{
	TerrainGen::TerrainGen(ChunkSize chunkSize, float minHeight, float varyHeight)
		: _chunkSize{ chunkSize }
	{
		_noise.SetSeed(42);
		_noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
		_noise.SetFractalType(FastNoiseLite::FractalType_FBm);

		_minHeight = minHeight;
		_varyHeight = varyHeight;

		VoxelSides voxelSides{};
		_voxelTypeLookUp[0] = VoxelType("air", false, voxelSides);

		voxelSides.frontTextureName = "bedrock";
		voxelSides.backTextureName = "bedrock";
		voxelSides.leftTextureName = "bedrock";
		voxelSides.rightTextureName = "bedrock";
		voxelSides.topTextureName = "bedrock";
		voxelSides.bottomTextureName = "bedrock";
		_voxelTypeLookUp[1] = VoxelType("bedrock", true, voxelSides);

		voxelSides.frontTextureName = "stone";
		voxelSides.backTextureName = "stone";
		voxelSides.leftTextureName = "stone";
		voxelSides.rightTextureName = "stone";
		voxelSides.topTextureName = "stone";
		voxelSides.bottomTextureName = "stone";
		_voxelTypeLookUp[2] = VoxelType("stone", true, voxelSides);

		voxelSides.frontTextureName = "dirt";
		voxelSides.backTextureName = "dirt";
		voxelSides.leftTextureName = "dirt";
		voxelSides.rightTextureName = "dirt";
		voxelSides.topTextureName = "dirt";
		voxelSides.bottomTextureName = "dirt";
		_voxelTypeLookUp[3] = VoxelType("dirt", true, voxelSides);

		voxelSides.frontTextureName = "grassSide";
		voxelSides.backTextureName = "grassSide";
		voxelSides.leftTextureName = "grassSide";
		voxelSides.rightTextureName = "grassSide";
		voxelSides.topTextureName = "grassTop";
		voxelSides.bottomTextureName = "dirt";
		_voxelTypeLookUp[4] = VoxelType("grass", true, voxelSides);

		createVoxelByteLookUp();
	}

	GLubyte TerrainGen::getVoxel(const glm::vec3& position) const
	{
		int y = static_cast<int>(floor(position.y));

		if (y == 0)
		{
			return _voxelByteLookUp.at("bedrock");
		}

		/* TERRAIN GENERATION */

		int height = static_cast<int>(floor(_noise.GetNoise(position.x, position.z) * _varyHeight + _minHeight));

		if (y > height)
		{
			return _voxelByteLookUp.at("air");
		}
		else if (y >= height)
		{
			return _voxelByteLookUp.at("grass");
		}
		else if (y >= height - 6)
		{
			return _voxelByteLookUp.at("dirt");
		}
		else
		{
			return _voxelByteLookUp.at("stone");
		}
	}

	VoxelType TerrainGen::getVoxelType(GLubyte byte) const
	{
		return _voxelTypeLookUp.at(byte);
	}

	void TerrainGen::createVoxelByteLookUp()
	{
		for (const auto& pair : _voxelTypeLookUp)
		{
			_voxelByteLookUp[pair.second.getName()] = pair.first;
		}
	}
}
