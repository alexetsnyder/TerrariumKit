#pragma once

#include "ICamera.h"

#include <glm/glm.hpp>

class FirstPersonCamera : public ICamera
{
	public:
		FirstPersonCamera(glm::vec3 position, glm::vec3 up);
		FirstPersonCamera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch, 
						  float speed, float sensitivity, float zoom);
		~FirstPersonCamera() {};

		glm::vec3 position() const override;
		glm::mat4 viewMatrix() const override;
		float zoom() const override;

		void move(CameraDirection direction, double deltaTime) override;
		void rotate(float xOffset, float yOffset) override;
		void zoom(float yOffset) override;

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

