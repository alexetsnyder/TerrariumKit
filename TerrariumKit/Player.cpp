#include "Player.h"

#include "MoveCommand.h"
#include "Time.h"

#include <iostream>

Player::Player(ICamera* camera)
	: camera_{ camera }, position_{ camera->position() }
{
	position_.y -= height;
}

Player::~Player()
{
}

void Player::move(InputDirection direction)
{
	double deltaTime = SysTK::Time::deltaTime();

	//IGameActor* pointer = dynamic_cast<IGameActor*>(camera_);
	//CmdTK::MoveCommand(pointer, direction).execute();

	glm::vec3 cameraPos = camera_->position();
	position_ = glm::vec3(cameraPos.x, cameraPos.y - height, cameraPos.z);
}

void Player::update()
{
}

void Player::draw(const ShaderProgram& program)
{
}
