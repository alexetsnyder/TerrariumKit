#pragma once

#include <map>
#include <string>
#include <vector>

class TextureAtlas
{
	public:
		TextureAtlas(int width, int blockSize);

		void createAtlas(std::vector<std::string> blockNames);

		std::vector<float> getTextureCoordinates(std::string name) const;

	private:
		std::vector<float> getTextureCoordinates(int index) const;

		int blockSize_;
		int atlasWidth_;
		int blockCount_;
		float normalizedBlockSize_;
		std::map<std::string, int> atlas_;
};

