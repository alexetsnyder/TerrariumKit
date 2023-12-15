#include "TextureAtlas.h"

#include <vector>

TextureAtlas::TextureAtlas(int width, int blockSize)
{
	_blockSize = blockSize;
	_atlasWidth = width;
	_blockCount = _atlasWidth / _blockSize;
}

std::vector<float> TextureAtlas::getTextureCoordinates(std::string name)
{
	int index = _atlas[name];
	return getTextureCoordinates(index);
}

void TextureAtlas::createAtlas(std::vector<std::string> blockNames)
{
	for (int i = 0; i < blockNames.size(); i++)
	{
		_atlas[blockNames[i]] = i;
	}
}

std::vector<float> TextureAtlas::getTextureCoordinates(int index)
{
	std::vector<float> textureCoordinates{};

	float y = index / _blockCount;
	float x = index - _blockCount * y;

	float normalizedBlockSize = _blockSize / _atlasWidth;

	y *= normalizedBlockSize;
	x *= normalizedBlockSize;

	textureCoordinates.push_back(x);
	textureCoordinates.push_back(y);

	textureCoordinates.push_back(x + normalizedBlockSize);
	textureCoordinates.push_back(y);

	textureCoordinates.push_back(x);
	textureCoordinates.push_back(y + normalizedBlockSize);

	textureCoordinates.push_back(x + normalizedBlockSize);
	textureCoordinates.push_back(y + normalizedBlockSize);

	return textureCoordinates;
}


