#pragma once

#include "IMeshRenderer.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture.h"

#include "glm/glm.hpp"

namespace CompTK
{
	class MeshRenderer : public IMeshRenderer
	{
		public:
			MeshRenderer();
			MeshRenderer(const glm::vec3& position);
			~MeshRenderer();
			MeshRenderer(const MeshRenderer&) = delete;

			void init(const glm::vec3& position);
			void free() const;

			MeshRenderer* next();

			void setNext(MeshRenderer* next);
			void setPosition(const glm::vec3& position);

			//From CompTK::IMeshRenderer
			void sendData(const RenderTK::Mesh& mesh) override;
			void draw(const RenderTK::ShaderProgram& program) const override;

		private:
			void generateAll();
			void bindAll();
			void unbindAll();	

			glm::vec3 position_;

			GLuint vao_;
			GLuint vbo_;
			GLuint ebo_;
			GLuint indicesCount_;	
			MeshRenderer* next_;
	};
}
