#pragma once

#include "Enums.h"

#include <glm/glm.hpp>

class ICamera
{
	public:
		virtual glm::vec3 position() const = 0;
		virtual glm::mat4 viewMatrix() const = 0;
		virtual float zoom() const = 0;

		virtual void move(InputDirection direction, double deltaTime) = 0;
		virtual void rotate(float xOffset, float yOffset) = 0;
		virtual void zoom(float yOffset) = 0;

		virtual ~ICamera() {};
};

