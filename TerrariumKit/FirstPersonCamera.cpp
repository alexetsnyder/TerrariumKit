#include "FirstPersonCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

FirstPersonCamera::FirstPersonCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f))
	: _position(position), _worldUp(up), _yaw(-90.0f), _pitch(0.0f), _speed(8.0f), _sensitivity(0.1f), _zoom(45.0f)
{
	updateVectors();
}

FirstPersonCamera::FirstPersonCamera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch, 
	                                 float speed, float sensitivity, float zoom)
	: _position(position), _worldUp(worldUp), _yaw(yaw), _pitch(pitch),
	  _speed(speed), _sensitivity(sensitivity), _zoom(zoom)
{
	updateVectors();
}

glm::vec3 FirstPersonCamera::position() const
{
	return _position;
}

glm::mat4 FirstPersonCamera::viewMatrix() const
{
	return glm::lookAt(_position, _position + _front, _up);
}

float FirstPersonCamera::zoom() const
{
	return _zoom;
}

void FirstPersonCamera::move(CameraDirection direction, double deltaTime)
{
	float velocity = static_cast<float>(_speed * deltaTime);
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

void FirstPersonCamera::rotate(float xOffset, float yOffset)
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

void FirstPersonCamera::zoom(float yOffset)
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

void FirstPersonCamera::updateVectors()
{
	glm::vec3 front{};
	front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	front.y = sin(glm::radians(_pitch));
	front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_front = glm::normalize(front);

	_right = glm::normalize(glm::cross(_front, _worldUp));
	_up = glm::normalize(glm::cross(_right, _front));
}
