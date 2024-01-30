#pragma once

#include "Enums.h"
#include "ICamera.h"

#include <glm/glm.hpp>

class TopDownCamera : public ICamera
{
	public:
		TopDownCamera(glm::vec3 position, glm::vec3 worldUp, float speed, float zoom);

		glm::vec3 position() const override;
		glm::mat4 viewMatrix() const override;
		float zoom() const override;

		void move(InputDirection direction, double deltaTime) override;
		void rotate(float xOffset, float yOffset) override;
		void zoom(float yOffset) override;

	private:
		void updateVectors();

		glm::vec3 _position;
		glm::vec3 _up;
		glm::vec3 _front;
		glm::vec3 _right;
		glm::vec3 _worldUp;

		float _speed;
		float _zoom;		
};

