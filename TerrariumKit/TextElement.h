#pragma once

#include "ITextRenderer.h"
#include "ShaderProgram.h"

#include <SDL/SDL.h>

#include <string>

namespace TextTK
{
	class TextElement
	{
		public:
			TextElement(std::string text, ITextRenderer* textRenderer);
			~TextElement();
			TextElement(const TextElement&) = delete;

			void draw(const RenderTK::ShaderProgram& program);

		private:
			std::string text_;
			ITextRenderer* textRenderer_;
	};
}
