#include "VoxelType.h"

namespace ProcGenTK
{
	VoxelType::VoxelType()
	{
		_isSolid = false;
	}

	VoxelType::VoxelType(const std::string& name, bool isSolid, const std::vector<std::string>& blockSidesVct)
	{
		setName(name);
		setIsSolid(isSolid);
		setBlockSides(blockSidesVct);
	}

	VoxelType::VoxelType(const std::string& name, bool isSolid, BlockSides blockSides)
	{
		setName(name);
		setIsSolid(isSolid);
		setBlockSides(blockSides);
	}

	bool VoxelType::isSolid() const
	{
		return _isSolid;
	}

	std::string VoxelType::getName() const
	{
		return _name;
	}

	BlockSides VoxelType::getBlockSides() const
	{
		return _blockSides;
	}

	void VoxelType::setIsSolid(bool isSolid)
	{
		_isSolid = isSolid;
	}

	void VoxelType::setName(const std::string& name)
	{
		_name = name;
	}

	void VoxelType::setBlockSides(BlockSides blockSides)
	{
		_blockSides = blockSides;
	}

	void VoxelType::setBlockSides(const std::vector<std::string>& blockSidesVct)
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
}
