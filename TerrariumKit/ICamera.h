#pragma once

#include <glm/glm.hpp>

enum CameraDirection
{
	FORWARD,
	BACKWARD,
	RIGHT,
	LEFT,
};

class ICamera
{
	public:
		~ICamera() {};

		virtual glm::vec3 position() const = 0;
		virtual glm::mat4 viewMatrix() = 0;
		virtual float zoom() = 0;

		virtual void move(CameraDirection direction, double deltaTime) = 0;
		virtual void lookAt(float xOffset, float yOffset) = 0;
		virtual void zoom(float yOffset) = 0;
};

