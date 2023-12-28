#include "BlockType.h"

BlockType::BlockType()
{
	_isSolid = false;
}

BlockType::BlockType(std::string name, bool isSolid, std::vector<std::string> blockSidesVct)
{
	setName(name);
	setIsSolid(isSolid);
	setBlockSides(blockSidesVct);
}

BlockType::BlockType(std::string name, bool isSolid, BlockSides blockSides)
{
	setName(name);
	setIsSolid(isSolid);
	setBlockSides(blockSides);
}

bool BlockType::isSolid() const
{
	return _isSolid;
}

std::string BlockType::getName() const
{
	return _name;
}

BlockSides BlockType::getBlockSides() const
{
	return _blockSides;
}

void BlockType::setIsSolid(bool isSolid)
{
	_isSolid = isSolid;
}

void BlockType::setName(std::string name)
{
	_name = name;
}

void BlockType::setBlockSides(BlockSides blockSides)
{
	_blockSides = blockSides;
}

void BlockType::setBlockSides(std::vector<std::string> blockSidesVct)
{
	BlockSides blockSides;
	blockSides.frontTextureName = blockSidesVct[0];
	blockSides.backTextureName = blockSidesVct[1];
	blockSides.leftTextureName = blockSidesVct[2];
	blockSides.rightTextureName = blockSidesVct[3];
	blockSides.topTextureName = blockSidesVct[4];
	blockSides.bottomTextureName = blockSidesVct[5];
	setBlockSides(blockSides);
}

