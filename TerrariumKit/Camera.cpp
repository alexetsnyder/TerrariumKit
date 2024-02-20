#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(CompTK::TransformComponent transform, float zoom)
	: transform_{ transform }, zoom_{ zoom }
{
	transform_.clampPitch(-89.0f, 89.0f);
}

glm::mat4 Camera::viewMatrix() const
{
	return glm::lookAt(transform_.position(), transform_.position() + transform_.front(), transform_.up());
}

float Camera::zoom() const
{
	return zoom_;
}

void Camera::zoom(float yOffset)
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

CompTK::TransformComponent& Camera::transform()
{
	return transform_;
}

const CompTK::TransformComponent& Camera::transform() const
{
	return transform_;
}
