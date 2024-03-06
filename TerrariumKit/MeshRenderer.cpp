#include "MeshRenderer.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

namespace CompTK
{
	MeshRenderer::MeshRenderer(const char* textureFile)
        : vao_{ 0 }, ebo_{ 0 }, vbo_{ 0 }, 
          indicesCount_{ 0 }, texture_{ textureFile, 
          TextureSettings{ GL_REPEAT, GL_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST } }
	{

	}

	MeshRenderer::~MeshRenderer()
	{
		free();
	}

	void MeshRenderer::sendData(const Mesh& mesh)
	{
        indicesCount_ = static_cast<GLuint>(mesh.getIndices().size());

        generateAll();
        bindAll();

        glBufferData(GL_ARRAY_BUFFER, mesh.getVertices().size() * sizeof(Vertex), &mesh.getVertices().front(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoordinate));
        glEnableVertexAttribArray(1);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndices().size() * sizeof(float), &mesh.getIndices().front(), GL_STATIC_DRAW);

        unbindAll();
	}

	void MeshRenderer::draw(const ShaderProgram& program, const glm::vec3& position) const
	{
        glm::mat4 model{ 1.0f };
        model = glm::translate(model, position);
        program.setUniform("model", model);

        texture_.bind();

        glBindVertexArray(vao_);
        glDrawElements(GL_TRIANGLES, indicesCount_, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
	}

    void MeshRenderer::generateAll()
    {
        glGenVertexArrays(1, &vao_);
        glGenBuffers(1, &vbo_);
        glGenBuffers(1, &ebo_);
    }

    void MeshRenderer::bindAll()
    {
        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    }

    void MeshRenderer::unbindAll()
    {
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void MeshRenderer::free() const
    {
        glDeleteVertexArrays(1, &vao_);
        glDeleteBuffers(1, &vbo_);
        glDeleteBuffers(1, &ebo_);
    }
}
