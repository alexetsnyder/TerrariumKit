#pragma once

#include "ICamera.h"
#include "ShaderProgram.h"

#include "glm/glm.hpp"

enum class PlayerDirection
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
};

class Player
{
	public:
		const float height{ 1.8f };

		Player(ICamera* camera);
		~Player();

		void move(PlayerDirection direction, double deltaTime);

		void update();
		void draw(const ShaderProgram& program);

	private:
		CameraDirection CameraDirectionFromPlayerDirection(PlayerDirection direction);

		ICamera* _camera;
		glm::vec3 _position;
};

