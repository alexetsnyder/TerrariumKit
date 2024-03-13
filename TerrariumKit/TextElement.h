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
			TextElement(const std::string& text, ITextRenderer* textRenderer);
			~TextElement();
			TextElement(const TextElement&) = delete;

			void setVisible(bool isVisible);
			void toggleVisible();

			void updateStr(const std::string& newStr);
			void draw(const RenderTK::ShaderProgram& program);

		private:
			bool isVisible_;
			std::string text_;
			ITextRenderer* textRenderer_;
	};
}
