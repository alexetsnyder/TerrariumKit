#include "TopDownCamera.h"

#include "Time.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

TopDownCamera::TopDownCamera(glm::vec3 position, glm::vec3 worldUp, float speed, float zoom)
	: position_{ position }, worldUp_{ worldUp }, speed_{ speed }, zoom_{ zoom }
{
	updateVectors();
}

glm::vec3 TopDownCamera::position() const
{
	return position_;
}

glm::mat4 TopDownCamera::viewMatrix() const
{
	return glm::lookAt(position_, position_ + front_, up_);
}

float TopDownCamera::zoom() const
{
	return zoom_;
}

void TopDownCamera::zoom(float yOffset)
{
	zoom_ -= yOffset;
	if (zoom_ < 1.0f)
	{
		zoom_ = 1.0f;
	}
	else if (zoom_ > 45.0f)
	{
		zoom_ = 45.0f;
	}
}

void TopDownCamera::move(InputDirection direction)
{
	double deltaTime = SysTK::Time::deltaTime();

	float velocity = static_cast<float>(speed_ * deltaTime);
	switch (direction)
	{
		case InputDirection::FORWARD:
			position_ += glm::vec3(0.0f, 0.0f, -1.0f) * velocity;
			break;
		case InputDirection::BACKWARD:
			position_ -= glm::vec3(0.0f, 0.0f, -1.0f) * velocity;
			break;
		case InputDirection::LEFT:
			position_ -= right_ * velocity;
			break;
		case InputDirection::RIGHT:
			position_ += right_ * velocity;
			break;
	}
}

void TopDownCamera::updateVectors()
{
	front_ = -glm::normalize(worldUp_);
	right_ = glm::normalize(glm::vec3{ 1.0f, 0.0f, 0.0f });
	up_ = glm::normalize(glm::cross(right_, front_));
}
