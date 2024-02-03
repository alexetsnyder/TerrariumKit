#pragma once

#include <string>
#include <vector>

namespace ProcGenTK
{
	struct BlockSides
	{
		std::string frontTextureName;
		std::string backTextureName;
		std::string leftTextureName;
		std::string rightTextureName;
		std::string topTextureName;
		std::string bottomTextureName;
	};

	class BlockType
	{
		public:
			BlockType();

			//Expects vector of size 6 with strings to be in order:
			// Front, Back, Left, Right, Top, Bottom.
			BlockType(const std::string& name, bool isSolid, const std::vector<std::string>& blockSidesVct);

			BlockType(const std::string& name, bool isSolid, BlockSides blockSides);

			bool isSolid() const;
			std::string getName() const;
			BlockSides getBlockSides() const;

		private:
			void setIsSolid(bool isSolid);
			void setName(const std::string& name);
			void setBlockSides(BlockSides blockSides);
			void setBlockSides(const std::vector<std::string>& blockSidesVct);

			bool _isSolid;
			std::string _name;
			BlockSides _blockSides;
	};
}
