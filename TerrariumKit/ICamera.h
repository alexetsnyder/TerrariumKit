#pragma once

#include <glm/glm.hpp>

enum class CameraDirection
{
	FORWARD,
	BACKWARD,
	RIGHT,
	LEFT,
};

class ICamera
{
	public:
		virtual glm::vec3 position() const = 0;
		virtual glm::mat4 viewMatrix() const = 0;
		virtual float zoom() const = 0;

		virtual void move(CameraDirection direction, double deltaTime) = 0;
		virtual void rotate(float xOffset, float yOffset) = 0;
		virtual void zoom(float yOffset) = 0;

		virtual ~ICamera() {};
};

