#pragma once

#include "GlyphAtlas.h"
#include "ITextRenderer.h"
#include "QuadRenderer.h"
#include "Texture.h"
#include "ShaderProgram.h"

#include <glm/glm.hpp>

#include <string_view>

namespace TextTK
{
	struct Bounds
	{
		glm::vec3 position;
		int width;
		int height;
	};

	class TextRenderer : public ITextRenderer
	{
		public:
			TextRenderer(int fontType, const Bounds& bounds);
			~TextRenderer();
			TextRenderer(const TextRenderer&) = delete;

			//From TextTK::ITextRenderer
			void drawTextLine(std::string_view line, int x, int y) override;
			void drawTextWrapped(std::string_view text, int x, int y) override;
			void draw(const RenderTK::ShaderProgram& program) const override;

		private:
			void drawTextLine(std::string_view line, SDL_Surface* dstSurface, int x, int y);
			void drawTextWrapped(std::string_view text, int x, int y, int maxWidth, int maxHeight);
			void nextLine(std::string& line, int& lineWidth, int& y, int height, int maxHeight);
			void addWord(std::string& line, int& lineWidth, std::string& word, int& wordWidth);

			int fontType_;
			Bounds bounds_;
			GlyphAtlas atlas_;
			RenderTK::Texture textTexture_;
			RenderTK::QuadRenderer quad_;
	};
}
