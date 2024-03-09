#include "TextRenderer.h"

#include "Input.h"
#include "Time.h"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

namespace TextTK
{
    TextRenderer::TextRenderer(std::string text, int fontType, int width, int height)
        : fontType_{ fontType }, text_{ text }, atlas_{},
          textTexture_{ width, height,
                        RenderTK::TextureSettings{ GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR } },
          quad_{ glm::vec3(width / 2.0f + 10.0f, height / 2.0f, 0.0f), width,  height }
    {
        drawTextWrapped(text, 0, 5, width, height);
    }

    TextRenderer::~TextRenderer()
    {
        
    }

    void TextRenderer::drawTextLine(std::string_view line, SDL_Surface* dstSurface, int x, int y)
    {
        for (const auto& c : line)
        {
            SDL_Rect srcRect{ atlas_.getGlyph(fontType_, static_cast<int>(c)) };

            SDL_Rect dstRect
            {
                x,
                y,
                srcRect.w,
                srcRect.h
            };

            SDL_BlitSurface(atlas_.getSurface(fontType_), &srcRect, dstSurface, &dstRect);

            x += srcRect.w;
        }
    }

    void TextRenderer::drawTextWrapped(std::string_view text, int x, int y, int maxWidth, int maxHeight)
    {
        SDL_Surface* dstSurface = SDL_CreateRGBSurface(0, maxWidth, maxHeight, 32, 0, 0, 0, 0xff);

        int lineWidth{ 0 };
        std::string line{ "" };
        int wordWidth{ 0 };
        std::string word{ "" };

        for (int i = 0; i < text.length(); i++)
        {
            char c{ text.at(i) };

            SDL_Rect glyph{ atlas_.getGlyph(fontType_, static_cast<int>(c)) };

            wordWidth += glyph.w;
            if (c != ' ' && c != '\n')
            {
                word += c;
            }

            if (c == ' ' || c == '\n' || i >= text.length() - 1)
            {
                if (lineWidth + wordWidth >= maxWidth)
                {
                    drawTextLine(line, dstSurface, x, y);

                    line = "";
                    lineWidth = 0;

                    y += glyph.h;
                    assert(y + glyph.h <= maxHeight);
                }
                else if (lineWidth > 0)
                {
                    line += " ";
                }

                line += word;
                lineWidth += wordWidth;

                word = "";
                wordWidth = 0;
            }
        }

        if (line.length() > 0)
        {
            drawTextLine(line, dstSurface, x, y);
        }

        textTexture_.updateTexture(dstSurface);
        SDL_FreeSurface(dstSurface);
    }

    void TextRenderer::draw(const RenderTK::ShaderProgram& program) const
    {
        textTexture_.bind();
        quad_.draw(program);
    }
}