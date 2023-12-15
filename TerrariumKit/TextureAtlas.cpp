#include "TextureAtlas.h"

#include <vector>

TextureAtlas::TextureAtlas(int width, int blockSize)
{
	_blockSize = blockSize;
	_atlasWidth = width;
	_blockCount = _atlasWidth / _blockSize;
	_normalizedBlockSize = (float)_blockSize / (float)_atlasWidth;
}

void TextureAtlas::createAtlas(std::vector<std::string> blockNames)
{
	for (int i = 0; i < blockNames.size(); i++)
	{
		_atlas[blockNames[i]] = i;
	}
}

std::vector<float> TextureAtlas::getTextureCoordinates(std::string name)
{
	int index = _atlas[name];
	return getTextureCoordinates(index);
}

std::vector<float> TextureAtlas::getTextureCoordinates(int index)
{
	std::vector<float> textureCoordinates{};

	float y = index / _blockCount;
	float x = index - _blockCount * y;

	y *= _normalizedBlockSize;
	x *= _normalizedBlockSize;

	textureCoordinates.push_back(x);
	textureCoordinates.push_back(y + _normalizedBlockSize);

	textureCoordinates.push_back(x + _normalizedBlockSize);
	textureCoordinates.push_back(y + _normalizedBlockSize);

	textureCoordinates.push_back(x);
	textureCoordinates.push_back(y);

	textureCoordinates.push_back(x + _normalizedBlockSize);
	textureCoordinates.push_back(y);

	return textureCoordinates;
}


