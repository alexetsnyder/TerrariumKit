#pragma once

#include "ICamera.h"
#include "IChunkMediator.h"
#include "IGameObject.h"
#include "ShaderProgram.h"

#include <glm/glm.hpp>

class Player : public EngineTK::IGameObject
{
	public:
		const float cameraHeight{ 1.8f };
		const float height{ 2.0f };
		const float radius{ 0.15f };
		const float gravity{ -9.8f };
		const float jumpForce{ 5.0f };
		const double jumpCooldown{ 0.2 };

		Player(ICamera* camera, const ProcGenTK::IChunkMediator* mediator, float walkSpeed);
		~Player();

		glm::vec3 position() const;

		//From EngineTK::IGameObject
		void update() override;
		void draw(const ShaderProgram& program) override;

	private:
		void calculateVelocity();
		void checkForCollision();
		bool yCollision(float dy);
		bool xCollision(float dx);
		bool zCollision(float dz);
		void jump();
		void resetJump();

		ICamera* camera_;
		const ProcGenTK::IChunkMediator* chunkMediator_;
		float walkSpeed_;
		glm::vec3 velocity_;

		float verticalVelocity_;
		bool isGrounded_;
		bool isReadyToJump_;
};

