#pragma once

#include "TransformComponent.h"

#include <glm/glm.hpp>

class ICamera
{
	public:
		virtual ~ICamera() {};

		virtual glm::mat4 viewMatrix() const = 0;
		virtual float zoom() const = 0;

		virtual void zoom(float yOffset) = 0;

		virtual const CompTK::TransformComponent& transform() const = 0;
		virtual CompTK::TransformComponent& transform() = 0;	
};
