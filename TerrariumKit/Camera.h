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

		float zoom() const override
		{
			return zoom_;
		}

		void zoom(float yOffset) override;

		CompTK::TransformComponent& transform() override
		{
			return transform_;
		}

		const CompTK::TransformComponent& transform() const override
		{
			return transform_;
		}

	private:
		CompTK::TransformComponent transform_;
		float zoom_;
};
