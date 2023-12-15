#pragma once

#include <glm/glm.hpp>

enum CameraDirection
{
	FORWARD,
	BACKWARD,
	RIGHT,
	LEFT,
};

class Camera
{
	public:
		Camera(glm::vec3 position, glm::vec3 up);

		glm::mat4 getViewMatrix();

		void move(CameraDirection direction, double deltaTime);

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

