#pragma once

#include "ICamera.h"
#include "TransformComponent.h"

#include <glm/glm.hpp>

class Camera : public ICamera
{
	public:
		Camera(CompTK::TransformComponent transform, float zoom);
		~Camera() {};

		//From ICamera
		glm::mat4 viewMatrix() const override;
		float zoom() const override;

		void zoom(float yOffset) override;

		CompTK::TransformComponent& transform() override;
		const CompTK::TransformComponent& transform() const override;

	private:
		CompTK::TransformComponent transform_;
		float zoom_;
};
