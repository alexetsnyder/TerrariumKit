#pragma once

#include "ICamera.h"
#include "IGameObject.h"
#include "ShaderProgram.h"

#include <glm/glm.hpp>

class Player : public EngineTK::IGameObject
{
	public:
		const float height{ 1.8f };

		Player(ICamera* camera, float walkSpeed);
		~Player();

		//From EngineTK::IGameObject
		void update() override;
		void draw(const ShaderProgram& program) override;

	private:
		ICamera* camera_;
		float walkSpeed_;
		glm::vec3 velocity_;
};

