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

void TopDownCamera::move(InputDirection direction, double deltaTime)
{
	float velocity = static_cast<float>(_speed * deltaTime);
	switch (direction)
	{
		case InputDirection::FORWARD:
			_position += glm::vec3(0.0f, 0.0f, -1.0f) * velocity;
			break;
		case InputDirection::BACKWARD:
			_position -= glm::vec3(0.0f, 0.0f, -1.0f) * velocity;
			break;
		case InputDirection::LEFT:
			_position -= _right * velocity;
			break;
		case InputDirection::RIGHT:
			_position += _right * velocity;
			break;
	}
}

void TopDownCamera::updateVectors()
{
	_front = -glm::normalize(_worldUp);
	_right = glm::normalize(glm::vec3{ 1.0f, 0.0f, 0.0f });
	_up = glm::normalize(glm::cross(_right, _front));
}
