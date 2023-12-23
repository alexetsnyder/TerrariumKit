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

		int _blockSize;
		int _atlasWidth;
		int _blockCount;
		float _normalizedBlockSize;
		std::map<std::string, int> _atlas;
};

