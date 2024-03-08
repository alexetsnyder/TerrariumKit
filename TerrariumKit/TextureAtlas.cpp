#include "TextureAtlas.h"

namespace RenderTK
{
	TextureAtlas::TextureAtlas(int width, int voxelSize, const std::vector<std::string>& voxelNames)
	{
		voxelSize_ = voxelSize;
		atlasWidth_ = width;
		voxelCount_ = atlasWidth_ / voxelSize_;
		normalizedVoxelSize_ = (float)voxelSize_ / (float)atlasWidth_;
		createAtlas(voxelNames);
	}

	std::vector<float> TextureAtlas::getTextureCoordinates(std::string name) const
	{
		int index = atlas_.at(name);
		return getTextureCoordinates(index);
	}

	void TextureAtlas::createAtlas(std::vector<std::string> voxelNames)
	{
		for (int i = 0; i < voxelNames.size(); i++)
		{
			atlas_[voxelNames[i]] = i;
		}
	}

	std::vector<float> TextureAtlas::getTextureCoordinates(int index) const
	{
		std::vector<float> textureCoordinates{};

		float y = static_cast<float>(floor(static_cast<float>(index) / voxelCount_));
		float x = index - voxelCount_ * y;

		y *= normalizedVoxelSize_;
		x *= normalizedVoxelSize_;

		textureCoordinates.push_back(x);
		textureCoordinates.push_back(y + normalizedVoxelSize_);

		textureCoordinates.push_back(x + normalizedVoxelSize_);
		textureCoordinates.push_back(y + normalizedVoxelSize_);

		textureCoordinates.push_back(x);
		textureCoordinates.push_back(y);

		textureCoordinates.push_back(x + normalizedVoxelSize_);
		textureCoordinates.push_back(y);

		return textureCoordinates;
	}

}
