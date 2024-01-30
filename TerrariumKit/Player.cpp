#include "Player.h"

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
	_camera->move(direction, deltaTime);

	glm::vec3 cameraPos = _camera->position();
	_position = glm::vec3(cameraPos.x, cameraPos.y - height, cameraPos.z);
}

void Player::update()
{
}

void Player::draw(const ShaderProgram& program)
{
}
