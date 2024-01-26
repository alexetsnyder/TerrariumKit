#include "Player.h"

Player::Player(ICamera* camera, glm::vec3 position)
	: _camera{ camera }, _position{ position }
{

}

Player::~Player()
{
}

void Player::update()
{
}

void Player::draw(const ShaderProgram& program)
{
}
