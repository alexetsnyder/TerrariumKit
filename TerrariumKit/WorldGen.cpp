#include "WorldGen.h"

#include "BlockType.h"

WorldGen::WorldGen()
	: _chunkSize{}
{
	
}

WorldGen::WorldGen(ChunkSize chunkSize)
{
	init(chunkSize);
}

void WorldGen::init(ChunkSize chunkSize)
{
	_chunkSize = chunkSize;

	BlockSides blockSides{};
	_blockTypeLookUp[0] = BlockType("Air", false, blockSides);

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

GLubyte WorldGen::getVoxel(glm::vec3 position)
{
	int y = static_cast<int>(floor(position.y));

	if (y == 0)
	{
		return _blockByteLookUp["bedrock"];
	}
	else if (y > _chunkSize.height)
	{
		return _blockByteLookUp["air"];
	}
	else if (y >= _chunkSize.height - 1)
	{
		return _blockByteLookUp["grass"];
	}
	else if (y >= _chunkSize.height - 2)
	{
		return _blockByteLookUp["dirt"];
	}
	else
	{
		return _blockByteLookUp["stone"];
	}

	return 0;
}

BlockType WorldGen::getBlockType(GLubyte byte)
{
	return _blockTypeLookUp[byte];
}

void WorldGen::createBlockByteLookUp()
{
	for (auto& pair : _blockTypeLookUp)
	{
		_blockByteLookUp[pair.second.getName()] = pair.first;
	}
}
