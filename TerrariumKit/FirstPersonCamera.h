#pragma once

#include "ICamera.h"
#include "ShaderProgram.h"
#include "TransformComponent.h"

#include <glm/glm.hpp>

class FirstPersonCamera : public ICamera
{
	public:
		FirstPersonCamera(CompTK::TransformComponent transform, float speed, float sensitivity, float zoom);
		~FirstPersonCamera() {};

		//From ICamera
		glm::vec3 position() const override;
		glm::mat4 viewMatrix() const override;
		float zoom() const override;

		void translate(glm::vec3 translation) override;
		void rotate(float xOffset, float yOffset) override;
		void zoom(float yOffset) override;
		void update() override;

	private:
		CompTK::TransformComponent transform_;
		glm::vec3 velocity_;

		float speed_;
		float sensitivity_;
		float zoom_;
};
