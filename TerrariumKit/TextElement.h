#pragma once

#include "ShaderProgram.h"
#include "TextRenderer.h"

#include <SDL/SDL.h>

#include <string>

namespace TextTK
{
	class TextElement
	{
		public:
			TextElement(std::string text, TextRenderer* textRenderer);
			~TextElement();
			TextElement(const TextElement&) = delete;

			void draw(const RenderTK::ShaderProgram& program);

		private:
			std::string text_;
			TextRenderer* textRenderer_;
	};
}
