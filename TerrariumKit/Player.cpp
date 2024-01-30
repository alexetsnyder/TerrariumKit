#include "Player.h"

#include "MoveCommand.h"

#include <iostream>

Player::Player(ICamera* camera)
	: _camera{ camera }, _position{ camera->position() }
{
	_position.y -= height;
}

Player::~Player()
{
}

void Player::move(InputDirection direction, double deltaTime)
{
	IGameActor* pointer = dynamic_cast<IGameActor*>(_camera);
	CmdTK::MoveCommand(pointer, direction, deltaTime).execute();

	glm::vec3 cameraPos = _camera->position();
	_position = glm::vec3(cameraPos.x, cameraPos.y - height, cameraPos.z);
}

void Player::update()
{
}

void Player::draw(const ShaderProgram& program)
{
}
