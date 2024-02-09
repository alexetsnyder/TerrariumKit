#pragma once

#include "Enums.h"
#include "ICamera.h"
#include "IGameActor.h"
#include "ShaderProgram.h"

#include "glm/glm.hpp"

class Player : public IGameActor
{
	public:
		const float height{ 1.8f };

		Player(ICamera* camera);
		~Player();

		//From IGameActor
		void move(InputDirection direction) override;	
		void update() override;
		void draw(const ShaderProgram& program) override;

	private:
		ICamera* _camera;
		glm::vec3 _position;
};

