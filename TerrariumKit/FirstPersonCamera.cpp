#include "FirstPersonCamera.h"

#include "Input.h"
#include "Keybindings.h"
#include "Time.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

FirstPersonCamera::FirstPersonCamera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch, 
	                                 float speed, float sensitivity, float zoom)
	: position_(position), velocity_{ 0.0f }, worldUp_(worldUp), yaw_(yaw), pitch_(pitch),
	  speed_(speed), sensitivity_(sensitivity), zoom_(zoom)
{
	updateVectors();
}

void FirstPersonCamera::translate(glm::vec3 translation)
{
	position_ += translation;
}

glm::vec3 FirstPersonCamera::position() const
{
	return position_;
}

glm::mat4 FirstPersonCamera::viewMatrix() const
{
	return glm::lookAt(position_, position_ + front_, up_);
}

float FirstPersonCamera::zoom() const
{
	return zoom_;
}

void FirstPersonCamera::rotate(float xOffset, float yOffset)
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

void FirstPersonCamera::zoom(float yOffset)
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

void FirstPersonCamera::update()
{
	float xRel = SysTK::Input::getMouseAxis(MouseAxis::X_AXIS);
	float yRel = SysTK::Input::getMouseAxis(MouseAxis::Y_AXIS);
	rotate(xRel, -yRel);

	velocity_ = glm::vec3{ 0.0f };
	double deltaTime = SysTK::Time::deltaTime();
	float speed = static_cast<float>(speed_ * deltaTime);

	if (SysTK::Input::getKey(SysTK::Keybindings::upKey))
	{
		velocity_ += front_ * speed;
	}
	if (SysTK::Input::getKey(SysTK::Keybindings::downKey))
	{
		velocity_ += -front_ * speed;
	}
	if (SysTK::Input::getKey(SysTK::Keybindings::leftKey))
	{
		velocity_ += -right_ * speed;
	}
	if (SysTK::Input::getKey(SysTK::Keybindings::rightKey))
	{
		velocity_ += right_ * speed;
	}

	translate(velocity_);
}

void FirstPersonCamera::updateVectors()
{
	glm::vec3 front{};
	front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	front.y = sin(glm::radians(pitch_));
	front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	front_ = glm::normalize(front);

	right_ = glm::normalize(glm::cross(front_, worldUp_));
	up_ = glm::normalize(glm::cross(right_, front_));
}
