#pragma once

#include "Enums.h"
#include "ICamera.h"
#include "IGameActor.h"
#include "ShaderProgram.h"

#include <glm/glm.hpp>

class TopDownCamera : public ICamera, public IGameActor
{
	public:
		TopDownCamera(glm::vec3 position, glm::vec3 worldUp, float speed, float zoom);

		//From ICamera
		glm::vec3 position() const override;
		glm::mat4 viewMatrix() const override;
		float zoom() const override;

		void rotate(float xOffset, float yOffset) override {};
		void zoom(float yOffset) override;

		//From IGameActor
		void move(InputDirection direction) override;
		void update() override {};
		void draw(const ShaderProgram& program) override {};

	private:
		void updateVectors();

		glm::vec3 position_;
		glm::vec3 up_;
		glm::vec3 front_;
		glm::vec3 right_;
		glm::vec3 worldUp_;

		float speed_;
		float zoom_;		
};

