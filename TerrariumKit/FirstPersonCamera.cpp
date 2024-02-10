#include "FirstPersonCamera.h"

#include "Time.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

FirstPersonCamera::FirstPersonCamera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch, 
	                                 float speed, float sensitivity, float zoom)
	: _position(position), _worldUp(worldUp), _yaw(yaw), _pitch(pitch),
	  _speed(speed), _sensitivity(sensitivity), _zoom(zoom)
{
	updateVectors();
}

void FirstPersonCamera::translate(glm::vec3 translation)
{
	_position += translation;
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

void FirstPersonCamera::move(InputDirection direction)
{
	double deltaTime = SysTK::Time::deltaTime();
	float speed = static_cast<float>(_speed * deltaTime);

	glm::vec3 velocity{ 0.0f };
	switch (direction)
	{
		case InputDirection::FORWARD:
			//_position += _front * speed;
			velocity = _front * speed;
			break;
		case InputDirection::BACKWARD:
			//_position -= _front * speed;
			velocity = -_front * speed;
			break;
		case InputDirection::LEFT:
			//_position -= _right * speed;
			velocity = -_right * speed;
			break;
		case InputDirection::RIGHT:
			//_position += _right * speed;
			velocity = _right * speed;
			break;
	}

	translate(velocity);
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
