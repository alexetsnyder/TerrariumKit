#include "TextRenderer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace TextTK
{
    TextRenderer::TextRenderer(int fontType, const Bounds& bounds)
        : fontType_{ fontType }, bounds_{ bounds }, atlas_ {},
          textTexture_{ bounds.width, bounds.height,
                        RenderTK::TextureSettings{ GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR } },
          quad_{ bounds.position, bounds.width,  bounds.height }
    {
        
    }

    TextRenderer::~TextRenderer()
    {
        
    }

    void TextRenderer::drawTextLine(std::string_view line, int x, int y)
    {
        SDL_Surface* dstSurface = SDL_CreateRGBSurface(0, bounds_.width, bounds_.height, 32, 0, 0, 0, 0xff);

        drawTextLine(line, dstSurface, x, y);

        textTexture_.updateTexture(dstSurface);
        SDL_FreeSurface(dstSurface);
    }

    void TextRenderer::drawTextWrapped(std::string_view text, int x, int y)
    {
        drawTextWrapped(text, x, y, bounds_.width, bounds_.height);
    }

    void TextRenderer::draw(const RenderTK::ShaderProgram& program) const
    {
        textTexture_.bind();
        quad_.draw(program);
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
                    nextLine(line, lineWidth, y, glyph.h, maxHeight);

                    addWord(line, lineWidth, word, wordWidth);
                }
                else
                {
                    if (lineWidth > 0)
                    {
                        line += " ";
                    }

                    addWord(line, lineWidth, word, wordWidth);
                    
                    if (c == '\n')
                    {
                        drawTextLine(line, dstSurface, x, y);
                        nextLine(line, lineWidth, y, glyph.h, maxHeight);
                    }
                }
            }
        }

        if (line.length() > 0)
        {
            drawTextLine(line, dstSurface, x, y);
        }

        textTexture_.updateTexture(dstSurface);
        SDL_FreeSurface(dstSurface);
    }

    void TextRenderer::nextLine(std::string& line, int& lineWidth, int& y, int height, int maxHeight)
    {
        line = "";
        lineWidth = 0;

        y += height;
        assert(y + height <= maxHeight);
    }

    void TextRenderer::addWord(std::string& line, int& lineWidth, std::string& word, int& wordWidth)
    {
        line += word;
        lineWidth += wordWidth;

        word = "";
        wordWidth = 0;
    }
}