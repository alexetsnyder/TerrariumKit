#pragma once

#include "ShaderProgram.h"

namespace TextTK
{
	class ITextRenderer
	{
		public:
			virtual ~ITextRenderer() {};
			virtual void drawTextLine(std::string_view line, int x, int y) = 0;
			virtual void drawTextWrapped(std::string_view text, int x, int y) = 0;
			virtual void draw(const RenderTK::ShaderProgram& program) const = 0;
	};
}