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

		BlockSides blockSides{};
		_blockTypeLookUp[0] = VoxelType("air", false, blockSides);

		blockSides.frontTextureName = "bedrock";
		blockSides.backTextureName = "bedrock";
		blockSides.leftTextureName = "bedrock";
		blockSides.rightTextureName = "bedrock";
		blockSides.topTextureName = "bedrock";
		blockSides.bottomTextureName = "bedrock";
		_blockTypeLookUp[1] = VoxelType("bedrock", true, blockSides);

		blockSides.frontTextureName = "stone";
		blockSides.backTextureName = "stone";
		blockSides.leftTextureName = "stone";
		blockSides.rightTextureName = "stone";
		blockSides.topTextureName = "stone";
		blockSides.bottomTextureName = "stone";
		_blockTypeLookUp[2] = VoxelType("stone", true, blockSides);

		blockSides.frontTextureName = "dirt";
		blockSides.backTextureName = "dirt";
		blockSides.leftTextureName = "dirt";
		blockSides.rightTextureName = "dirt";
		blockSides.topTextureName = "dirt";
		blockSides.bottomTextureName = "dirt";
		_blockTypeLookUp[3] = VoxelType("dirt", true, blockSides);

		blockSides.frontTextureName = "grassSide";
		blockSides.backTextureName = "grassSide";
		blockSides.leftTextureName = "grassSide";
		blockSides.rightTextureName = "grassSide";
		blockSides.topTextureName = "grassTop";
		blockSides.bottomTextureName = "dirt";
		_blockTypeLookUp[4] = VoxelType("grass", true, blockSides);

		createBlockByteLookUp();
	}

	GLubyte TerrainGen::getVoxel(const glm::vec3& position) const
	{
		int y = static_cast<int>(floor(position.y));

		if (y == 0)
		{
			return _blockByteLookUp.at("bedrock");
		}

		/* TERRAIN GENERATION */

		int height = static_cast<int>(floor(_noise.GetNoise(position.x, position.z) * _varyHeight + _minHeight));

		if (y > height)
		{
			return _blockByteLookUp.at("air");
		}
		else if (y >= height)
		{
			return _blockByteLookUp.at("grass");
		}
		else if (y >= height - 6)
		{
			return _blockByteLookUp.at("dirt");
		}
		else
		{
			return _blockByteLookUp.at("stone");
		}
	}

	VoxelType TerrainGen::getBlockType(GLubyte byte) const
	{
		return _blockTypeLookUp.at(byte);
	}

	void TerrainGen::createBlockByteLookUp()
	{
		for (const auto& pair : _blockTypeLookUp)
		{
			_blockByteLookUp[pair.second.getName()] = pair.first;
		}
	}
}
