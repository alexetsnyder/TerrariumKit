#pragma once

#include "ICamera.h"

#include <glm/glm.hpp>

class Camera
{
	public:
		Camera(glm::vec3 position, glm::vec3 up);

		glm::vec3 position() const;
		glm::mat4 viewMatrix() const;
		float zoom() const;

		void move(CameraDirection direction, double deltaTime);
		void rotate(float xOffset, float yOffset);
		void zoom(float yOffset);

	private:
		void updateVectors();

		glm::vec3 _position;
		glm::vec3 _up;
		glm::vec3 _front;
		glm::vec3 _right;
		glm::vec3 _worldUp;

		float _yaw;
		float _pitch;
		float _speed;
		float _sensitivity;
		float _zoom;
};

