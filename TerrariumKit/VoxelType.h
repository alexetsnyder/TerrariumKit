#pragma once

#include <string>
#include <vector>

namespace ProcGenTK
{
	struct VoxelSides
	{
		std::string frontTextureName;
		std::string backTextureName;
		std::string leftTextureName;
		std::string rightTextureName;
		std::string topTextureName;
		std::string bottomTextureName;
	};

	class VoxelType
	{
		public:
			VoxelType();

			//Expects vector of size 6 with strings to be in order:
			// Front, Back, Left, Right, Top, Bottom.
			VoxelType(const std::string& name, bool isSolid, const std::vector<std::string>& voxelSidesVct);

			VoxelType(const std::string& name, bool isSolid, VoxelSides voxelSides);

			bool isSolid() const;
			std::string getName() const;
			VoxelSides getVoxelSides() const;

		private:
			void setIsSolid(bool isSolid);
			void setName(const std::string& name);
			void setVoxelSides(VoxelSides voxelSides);
			void setVoxelSides(const std::vector<std::string>& voxelSidesVct);

			bool _isSolid;
			std::string _name;
			VoxelSides _voxelSides;
	};
}
