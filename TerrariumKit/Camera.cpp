#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f))
	: _position(position), _worldUp(up), _yaw(-90.0f), _pitch(0.0f), _speed(6.0f), _sensitivity(0.1f), _zoom(45.0f)
{
	updateVectors();
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(_position, _position + _front, _up);
}

float Camera::getZoom()
{
	return _zoom;
}

void Camera::move(CameraDirection direction, double deltaTime)
{
	float velocity = _speed * deltaTime;
	switch (direction)
	{
		case CameraDirection::FORWARD:
			_position += _front * velocity;
			break;
		case CameraDirection::BACKWARD:
			_position -= _front * velocity;
			break;
		case CameraDirection::LEFT:
			_position -= _right * velocity;
			break;
		case CameraDirection::RIGHT:
			_position += _right * velocity;
			break;
	}
}

void Camera::lookAt(float xOffset, float yOffset)
{
	xOffset *= _sensitivity;
	yOffset *= _sensitivity;

	_yaw += xOffset;
	_pitch += yOffset;

	if (_pitch > 89.0f)
	{
		_pitch = 89.0f;
	}
	else if (_pitch < -89.0f)
	{
		_pitch = -89.0f;
	}

	updateVectors();
}

void Camera::zoom(float yOffset)
{
	_zoom -= yOffset;
	if (_zoom < 1.0f)
	{
		_zoom = 1.0f;
	}
	else if (_zoom > 45.0f)
	{
		_zoom = 45.0f;
	}
}

void Camera::updateVectors()
{
	glm::vec3 front{};
	front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	front.y = sin(glm::radians(_pitch));
	front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_front = glm::normalize(front);

	_right = glm::normalize(glm::cross(_front, _worldUp));
	_up = glm::normalize(glm::cross(_right, _front));
}
