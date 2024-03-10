#include "TextElement.h"

namespace TextTK
{
	TextElement::TextElement(std::string text, TextRenderer* textRenderer)
		: text_{ text }, textRenderer_{ textRenderer }
	{
		textRenderer_->drawTextWrapped(text, 5, 5);
	}

	TextElement::~TextElement()
	{
		delete textRenderer_;
	}

	void TextElement::draw(const RenderTK::ShaderProgram& program)
	{
		textRenderer_->draw(program);
	}
}