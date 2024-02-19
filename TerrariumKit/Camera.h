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

		//From ICamera unused
		glm::vec3 position() const override { return transform_.position(); }
		void translate(glm::vec3 translation) override {};
		void rotate(float xOffset, float yOffset) override {};
		void update() override {};

	private:
		CompTK::TransformComponent transform_;
		float zoom_;
};
