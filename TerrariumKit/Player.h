#pragma once

#include "ICamera.h"
#include "ShaderProgram.h"

#include "glm/glm.hpp"

class Player
{
	public:
		Player(ICamera* camera, glm::vec3 position);
		~Player();

		void update();
		void draw(const ShaderProgram& program);

	private:
		const float _height{ 1.8f };
		ICamera* _camera;
		glm::vec3 _position;
};

