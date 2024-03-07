#include "TextRenderer.h"

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

    TextRenderer::TextRenderer(int width, int height)
        : vao_{ 0 }, vbo_{ 0 }, ebo_{ 0 }, model_{ 1.0f }, atlas_{}, 
          textTexture_{ atlas_.getSurface(FontType::Px437_IBM_VGA_8x14), 
                        TextureSettings{ GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR } }  
    {
        sendData();
        calculateModel(width, height);
    }

    TextRenderer::~TextRenderer()
    {

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

    void TextRenderer::draw(const ShaderProgram& program) const
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