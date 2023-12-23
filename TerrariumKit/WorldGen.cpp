#include "WorldGen.h"

#include "BlockType.h"

#include <iostream>

WorldGen::WorldGen()
	: _chunkSize{}, _noise{}
{
	_minHeight = 0;
	_varyHeight = 0;
}

WorldGen::WorldGen(ChunkSize chunkSize, int minHeight, int varyHeight)
{
	init(chunkSize, minHeight, varyHeight);
}

void WorldGen::init(ChunkSize chunkSize, int minHeight, int varyHeight)
{
	_chunkSize = chunkSize;

	_noise.SetSeed(0);
	_noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	_noise.SetFractalType(FastNoiseLite::FractalType_FBm);
	_minHeight = minHeight;
	_varyHeight = varyHeight;

	BlockSides blockSides{};
	_blockTypeLookUp[0] = BlockType("air", false, blockSides);

	blockSides.frontTextureName = "bedrock";
	blockSides.backTextureName = "bedrock";
	blockSides.leftTextureName = "bedrock";
	blockSides.rightTextureName = "bedrock";
	blockSides.topTextureName = "bedrock";
	blockSides.bottomTextureName = "bedrock";
	_blockTypeLookUp[1] = BlockType("bedrock", true, blockSides);

	blockSides.frontTextureName = "stone";
	blockSides.backTextureName = "stone";
	blockSides.leftTextureName = "stone";
	blockSides.rightTextureName = "stone";
	blockSides.topTextureName = "stone";
	blockSides.bottomTextureName = "stone";
	_blockTypeLookUp[2] = BlockType("stone", true, blockSides);

	blockSides.frontTextureName = "dirt";
	blockSides.backTextureName = "dirt";
	blockSides.leftTextureName = "dirt";
	blockSides.rightTextureName = "dirt";
	blockSides.topTextureName = "dirt";
	blockSides.bottomTextureName = "dirt";
	_blockTypeLookUp[3] = BlockType("dirt", true, blockSides);

	blockSides.frontTextureName = "grassSide";
	blockSides.backTextureName = "grassSide";
	blockSides.leftTextureName = "grassSide";
	blockSides.rightTextureName = "grassSide";
	blockSides.topTextureName = "grassTop";
	blockSides.bottomTextureName = "dirt";
	_blockTypeLookUp[4] = BlockType("grass", true, blockSides);

	createBlockByteLookUp();
}

GLubyte WorldGen::getVoxel(const glm::vec3& position) const
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

	return 0;
}

BlockType WorldGen::getBlockType(GLubyte byte) const
{
	return _blockTypeLookUp.at(byte);
}

void WorldGen::createBlockByteLookUp()
{
	for (auto& pair : _blockTypeLookUp)
	{
		_blockByteLookUp[pair.second.getName()] = pair.first;
	}
}
