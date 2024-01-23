#include "TopDownCamera.h"

glm::vec3 TopDownCamera::position() const
{
	return glm::vec3();
}

glm::mat4 TopDownCamera::viewMatrix() const
{
	return glm::mat4();
}

float TopDownCamera::zoom() const
{
	return 0.0f;
}

void TopDownCamera::move(CameraDirection direction, double deltaTime)
{
}

void TopDownCamera::rotate(float xOffset, float yOffset)
{
}

void TopDownCamera::zoom(float yOffset)
{
}
