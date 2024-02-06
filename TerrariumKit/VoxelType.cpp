#include "VoxelType.h"

namespace ProcGenTK
{
	VoxelType::VoxelType()
	{
		_isSolid = false;
	}

	VoxelType::VoxelType(const std::string& name, bool isSolid, const std::vector<std::string>& voxelSidesVct)
	{
		setName(name);
		setIsSolid(isSolid);
		setVoxelSides(voxelSidesVct);
	}

	VoxelType::VoxelType(const std::string& name, bool isSolid, VoxelSides voxelSides)
	{
		setName(name);
		setIsSolid(isSolid);
		setVoxelSides(voxelSides);
	}

	bool VoxelType::isSolid() const
	{
		return _isSolid;
	}

	std::string VoxelType::getName() const
	{
		return _name;
	}

	VoxelSides VoxelType::getVoxelSides() const
	{
		return _voxelSides;
	}

	void VoxelType::setIsSolid(bool isSolid)
	{
		_isSolid = isSolid;
	}

	void VoxelType::setName(const std::string& name)
	{
		_name = name;
	}

	void VoxelType::setVoxelSides(VoxelSides voxelSides)
	{
		_voxelSides = voxelSides;
	}

	void VoxelType::setVoxelSides(const std::vector<std::string>& voxelSidesVct)
	{
		VoxelSides voxelSides;
		voxelSides.frontTextureName = voxelSidesVct[0];
		voxelSides.backTextureName = voxelSidesVct[1];
		voxelSides.leftTextureName = voxelSidesVct[2];
		voxelSides.rightTextureName = voxelSidesVct[3];
		voxelSides.topTextureName = voxelSidesVct[4];
		voxelSides.bottomTextureName = voxelSidesVct[5];
		setVoxelSides(voxelSides);
	}
}
