#include "TopDownCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

TopDownCamera::TopDownCamera(glm::vec3 position, glm::vec3 worldUp, float speed, float zoom)
	: _position{ position }, _worldUp{ worldUp }, _speed{ speed }, _zoom{ zoom }
{
	updateVectors();
}

glm::vec3 TopDownCamera::position() const
{
	return _position;
}

glm::mat4 TopDownCamera::viewMatrix() const
{
	return glm::lookAt(_position, _position + _front, _up);
}

float TopDownCamera::zoom() const
{
	return _zoom;
}

void TopDownCamera::move(CameraDirection direction, double deltaTime)
{
	float velocity = static_cast<float>(_speed * deltaTime);
	switch (direction)
	{
		case CameraDirection::FORWARD:
			_position += _up * velocity;
			break;
		case CameraDirection::BACKWARD:
			_position -= _up * velocity;
			break;
		case CameraDirection::LEFT:
			_position -= _right * velocity;
			break;
		case CameraDirection::RIGHT:
			_position += _right * velocity;
			break;
	}
}

void TopDownCamera::rotate(float xOffset, float yOffset)
{
}

void TopDownCamera::zoom(float yOffset)
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

void TopDownCamera::updateVectors()
{
	_front = -glm::normalize(_worldUp);
	_right = glm::normalize(glm::vec3{ 1.0f, 0.0f, 0.0f });
	_up = glm::normalize(glm::cross(_right, _front));
}
