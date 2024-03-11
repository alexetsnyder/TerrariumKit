#include "TextElement.h"

namespace TextTK
{
	TextElement::TextElement(const std::string& text, ITextRenderer* textRenderer)
		: text_{ text }, textRenderer_{ textRenderer }
	{
		textRenderer_->drawTextWrapped(text_, 5, 5);
	}

	TextElement::~TextElement()
	{
		delete textRenderer_;
	}

	void TextElement::updateStr(const std::string& newStr)
	{
		text_ = newStr;
		textRenderer_->drawTextWrapped(text_, 5, 5);
	}

	void TextElement::draw(const RenderTK::ShaderProgram& program)
	{
		textRenderer_->draw(program);
	}
}