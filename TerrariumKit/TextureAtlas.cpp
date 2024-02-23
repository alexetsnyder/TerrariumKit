#include "TextureAtlas.h"

#include <vector>

TextureAtlas::TextureAtlas(int width, int blockSize)
{
	blockSize_ = blockSize;
	atlasWidth_ = width;
	blockCount_ = atlasWidth_ / blockSize_;
	normalizedBlockSize_ = (float)blockSize_ / (float)atlasWidth_;
}

void TextureAtlas::createAtlas(std::vector<std::string> blockNames)
{
	for (int i = 0; i < blockNames.size(); i++)
	{
		atlas_[blockNames[i]] = i;
	}
}

std::vector<float> TextureAtlas::getTextureCoordinates(std::string name) const
{
	int index = atlas_.at(name);
	return getTextureCoordinates(index);
}

std::vector<float> TextureAtlas::getTextureCoordinates(int index) const
{
	std::vector<float> textureCoordinates{};

	float y = static_cast<float>(floor(static_cast<float>(index) / blockCount_));
	float x = index - blockCount_ * y;

	y *= normalizedBlockSize_;
	x *= normalizedBlockSize_;

	textureCoordinates.push_back(x);
	textureCoordinates.push_back(y + normalizedBlockSize_);

	textureCoordinates.push_back(x + normalizedBlockSize_);
	textureCoordinates.push_back(y + normalizedBlockSize_);

	textureCoordinates.push_back(x);
	textureCoordinates.push_back(y);

	textureCoordinates.push_back(x + normalizedBlockSize_);
	textureCoordinates.push_back(y);

	return textureCoordinates;
}


