#pragma once

#include "ICamera.h"

class TopDownCamera : public ICamera
{
	public:
		glm::vec3 position() const override;
		glm::mat4 viewMatrix() const override;
		float zoom() const override;

		void move(CameraDirection direction, double deltaTime) override;
		void rotate(float xOffset, float yOffset) override;
		void zoom(float yOffset) override;

	private:	
		float _speed;
		float _sensitivity;
		float _zoom;		
};

