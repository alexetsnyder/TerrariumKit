#include "TextRenderer.h"

#include "Input.h"
#include "Time.h"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

namespace TextTK
{
	constexpr int VERTEX_COUNT{ 20 };
	constexpr int INDEX_COUNT{ 6 };

	const float quadVertexArray[VERTEX_COUNT]
	{
        //Positions           //Texture Coordinates
		0.5f,  0.5f,  0.0f,   1.0f, 0.0f,
	   -0.5f,  0.5f,  0.0f,   0.0f, 0.0f,
		0.5f, -0.5f,  0.0f,   1.0f, 1.0f,
	   -0.5f, -0.5f,  0.0f,   0.0f, 1.0f,
	};

	const int quadIndexArray[INDEX_COUNT]
	{
		0, 1, 2,
		2, 1, 3,
	};

    TextRenderer::TextRenderer(std::string text, int fontType, int width, int height)
        : vao_{ 0 }, vbo_{ 0 }, ebo_{ 0 }, model_{ 1.0f }, atlas_{}, fontType_{ fontType },
          textTexture_{ width, height,
                        RenderTK::TextureSettings{ GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR } },
          text_{ text }
    {
        textTexture_.updateTexture(drawText(text_, 0, 10, width, height));
        sendData();
        calculateModel(width, height);
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

    void TextRenderer::calculateModel(int width, int height)
    {
        model_ = glm::translate(model_, glm::vec3(width / 2.0f + 10.0f, height / 2.0f, 0.0f));
        model_ = glm::scale(model_, glm::vec3(width, height, 0.0f));
    }

    void TextRenderer::sendData()
    {
        generateAll();
        bindAll();

        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertexArray), quadVertexArray, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndexArray), quadIndexArray, GL_STATIC_DRAW);

        unbindAll();
    }

    void TextRenderer::draw(const RenderTK::ShaderProgram& program) const
    {
        textTexture_.bind();

        program.setUniform("model", model_);

        glBindVertexArray(vao_);
        glDrawElements(GL_TRIANGLES, INDEX_COUNT, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void TextRenderer::generateAll()
    {
        glGenVertexArrays(1, &vao_);
        glGenBuffers(1, &vbo_);
        glGenBuffers(1, &ebo_);
    }

    void TextRenderer::bindAll()
    {
        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    }

    void TextRenderer::unbindAll()
    {
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void TextRenderer::free() const
    {
        glDeleteVertexArrays(1, &vao_);
        glDeleteBuffers(1, &vbo_);
        glDeleteBuffers(1, &ebo_);
    }
}