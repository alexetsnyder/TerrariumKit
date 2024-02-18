#include "FirstPersonCamera.h"

#include "Input.h"
#include "Keybindings.h"
#include "Time.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

FirstPersonCamera::FirstPersonCamera(CompTK::TransformComponent transform, float speed, float sensitivity, float zoom)
	: transform_{ transform }, velocity_{ 0.0f }, speed_{ speed }, sensitivity_{ sensitivity }, zoom_{ zoom }
{
	transform_.clampPitch(-89.0f, 89.0f);
}

glm::vec3 FirstPersonCamera::position() const
{
	return transform_.position();
}

glm::mat4 FirstPersonCamera::viewMatrix() const
{
	return glm::lookAt(transform_.position(), transform_.position() + transform_.front(), transform_.up());
}

float FirstPersonCamera::zoom() const
{
	return zoom_;
}

void FirstPersonCamera::translate(glm::vec3 translation)
{
	transform_.translate(translation);
}

void FirstPersonCamera::rotate(float xOffset, float yOffset)
{
	xOffset *= sensitivity_;
	yOffset *= sensitivity_;

	transform_.rotate(xOffset, yOffset);
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

	float yWheel = SysTK::Input::getMouseWheel();
	zoom(yWheel);

	velocity_ = glm::vec3{ 0.0f };
	double deltaTime = SysTK::Time::deltaTime();
	float speed = static_cast<float>(speed_ * deltaTime);

	if (SysTK::Input::getKey(SysTK::Keybindings::upKey))
	{
		velocity_ += transform_.front() * speed;
	}
	if (SysTK::Input::getKey(SysTK::Keybindings::downKey))
	{
		velocity_ += -transform_.front() * speed;
	}
	if (SysTK::Input::getKey(SysTK::Keybindings::leftKey))
	{
		velocity_ += -transform_.right() * speed;
	}
	if (SysTK::Input::getKey(SysTK::Keybindings::rightKey))
	{
		velocity_ += transform_.right() * speed;
	}

	translate(velocity_);
}
