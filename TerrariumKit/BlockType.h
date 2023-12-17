#pragma once

#include <string>
#include <vector>

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
		BlockType(std::string name, bool isSolid, std::vector<std::string> blockSidesVct);

		BlockType(std::string name, bool isSolid, BlockSides blockSides);

		bool isSolid();
		std::string getName();
		BlockSides getBlockSides();

		void setIsSolid(bool isSolid);
		void setName(std::string name);
		void setBlockSides(BlockSides blockSides);

		//Expects vector of size 6 with strings to be in order:
		// Front, Back, Left, Right, Top, Bottom.
		void setBlockSides(std::vector<std::string> blockSidesVct);

	private:
		bool _isSolid;
		std::string _name;	
		BlockSides _blockSides;
};


