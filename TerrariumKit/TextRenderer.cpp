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
        textTexture_.updateTexture(drawText(text_, 0, 10, width, height));
    }

    TextRenderer::~TextRenderer()
    {
        
    }

    SDL_Surface* TextRenderer::drawText(std::string_view text, int x, int y, int width, int height)
    {
        SDL_Surface* dstSurface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0xff);

        for (const auto& c : text)
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

        return dstSurface;
    }

    void TextRenderer::draw(const RenderTK::ShaderProgram& program) const
    {
        textTexture_.bind();
        quad_.draw(program);
    }
}