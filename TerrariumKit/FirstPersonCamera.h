#pragma once

#include "ICamera.h"
#include "ShaderProgram.h"

#include <glm/glm.hpp>

class FirstPersonCamera : public ICamera
{
	public:
		FirstPersonCamera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch, 
						  float speed, float sensitivity, float zoom);
		~FirstPersonCamera() {};

		void translate(glm::vec3 translation);

		//From ICamera
		glm::vec3 position() const override;
		glm::mat4 viewMatrix() const override;
		float zoom() const override;

		void rotate(float xOffset, float yOffset) override;
		void zoom(float yOffset) override;
		void update() override;

	private:
		void updateVectors();

		glm::vec3 position_;
		glm::vec3 velocity_;
		glm::vec3 up_;
		glm::vec3 front_;
		glm::vec3 right_;
		glm::vec3 worldUp_;

		float yaw_;
		float pitch_;
		float speed_;
		float sensitivity_;
		float zoom_;
};

