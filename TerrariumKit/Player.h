#pragma once

#include "Enums.h"
#include "ICamera.h"
#include "ShaderProgram.h"

#include "glm/glm.hpp"

class Player
{
	public:
		const float height{ 1.8f };

		Player(ICamera* camera);
		~Player();

		void move(InputDirection direction, double deltaTime);	

		void update();
		void draw(const ShaderProgram& program);

	private:
		ICamera* _camera;
		glm::vec3 _position;
};

