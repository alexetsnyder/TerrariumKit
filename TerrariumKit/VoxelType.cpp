#include "VoxelType.h"

namespace ProcGenTK
{
	VoxelType::VoxelType()
	{
		isSolid_ = false;
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
		return isSolid_;
	}

	std::string VoxelType::getName() const
	{
		return name_;
	}

	VoxelSides VoxelType::getVoxelSides() const
	{
		return voxelSides_;
	}

	void VoxelType::setIsSolid(bool isSolid)
	{
		isSolid_ = isSolid;
	}

	void VoxelType::setName(const std::string& name)
	{
		name_ = name;
	}

	void VoxelType::setVoxelSides(VoxelSides voxelSides)
	{
		voxelSides_ = voxelSides;
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
