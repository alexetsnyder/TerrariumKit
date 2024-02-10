#include "FlyingCamera.h"

#include "Time.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

FlyingCamera::FlyingCamera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch,
	float speed, float sensitivity, float zoom)
	: _position(position), _worldUp(worldUp), _yaw(yaw), _pitch(pitch),
	_speed(speed), _sensitivity(sensitivity), _zoom(zoom)
{
	updateVectors();
}

glm::vec3 FlyingCamera::position() const
{
	return _position;
}

glm::mat4 FlyingCamera::viewMatrix() const
{
	return glm::lookAt(_position, _position + _front, _up);
}

float FlyingCamera::zoom() const
{
	return _zoom;
}

void FlyingCamera::rotate(float xOffset, float yOffset)
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

void FlyingCamera::zoom(float yOffset)
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

void FlyingCamera::move(InputDirection direction)
{
	double deltaTime = SysTK::Time::deltaTime();

	float velocity = static_cast<float>(_speed * deltaTime);
	switch (direction)
	{
		case InputDirection::FORWARD:
			_position += _front * velocity;
			break;
		case InputDirection::BACKWARD:
			_position -= _front * velocity;
			break;
		case InputDirection::LEFT:
			_position -= _right * velocity;
			break;
		case InputDirection::RIGHT:
			_position += _right * velocity;
			break;
	}
}

void FlyingCamera::updateVectors()
{
	glm::vec3 front{};
	front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	front.y = sin(glm::radians(_pitch));
	front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_front = glm::normalize(front);

	_right = glm::normalize(glm::cross(_front, _worldUp));
	_up = glm::normalize(glm::cross(_right, _front));
}