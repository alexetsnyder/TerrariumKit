#include "FlyingCamera.h"

#include "Time.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

FlyingCamera::FlyingCamera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch,
	float speed, float sensitivity, float zoom)
	: position_(position), worldUp_(worldUp), yaw_(yaw), pitch_(pitch),
	speed_(speed), sensitivity_(sensitivity), zoom_(zoom)
{
	updateVectors();
}

glm::vec3 FlyingCamera::position() const
{
	return position_;
}

glm::mat4 FlyingCamera::viewMatrix() const
{
	return glm::lookAt(position_, position_ + front_, up_);
}

float FlyingCamera::zoom() const
{
	return zoom_;
}

void FlyingCamera::rotate(float xOffset, float yOffset)
{
	xOffset *= sensitivity_;
	yOffset *= sensitivity_;

	yaw_ += xOffset;
	pitch_ += yOffset;

	if (pitch_ > 89.0f)
	{
		pitch_ = 89.0f;
	}
	else if (pitch_ < -89.0f)
	{
		pitch_ = -89.0f;
	}

	updateVectors();
}

void FlyingCamera::zoom(float yOffset)
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

void FlyingCamera::move(InputDirection direction)
{
	double deltaTime = SysTK::Time::deltaTime();

	float velocity = static_cast<float>(speed_ * deltaTime);
	switch (direction)
	{
		case InputDirection::FORWARD:
			position_ += front_ * velocity;
			break;
		case InputDirection::BACKWARD:
			position_ -= front_ * velocity;
			break;
		case InputDirection::LEFT:
			position_ -= right_ * velocity;
			break;
		case InputDirection::RIGHT:
			position_ += right_ * velocity;
			break;
	}
}

void FlyingCamera::updateVectors()
{
	glm::vec3 front{};
	front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	front.y = sin(glm::radians(pitch_));
	front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	front_ = glm::normalize(front);

	right_ = glm::normalize(glm::cross(front_, worldUp_));
	up_ = glm::normalize(glm::cross(right_, front_));
}
