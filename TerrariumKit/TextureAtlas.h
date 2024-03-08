#pragma once

#include <map>
#include <string>
#include <vector>

namespace RenderTK
{
	class TextureAtlas
	{
		public:
			TextureAtlas(int width, int voxelSize, const std::vector<std::string>& voxelNames);

			std::vector<float> getTextureCoordinates(std::string name) const;

		private:
			void createAtlas(std::vector<std::string> voxelNames);
			std::vector<float> getTextureCoordinates(int index) const;

			int voxelSize_;
			int atlasWidth_;
			int voxelCount_;
			float normalizedVoxelSize_;
			std::map<std::string, int> atlas_;
	};
}
