#include "Player.h"

#include "Time.h"

Player::Player(ICamera* camera)
	: camera_{ camera }, position_{ camera->transform().position() }
{
	position_.y -= height;
}

Player::~Player()
{
}

void Player::move(InputDirection direction)
{
	double deltaTime = SysTK::Time::deltaTime();

	glm::vec3 cameraPos = camera_->transform().position();
	position_ = glm::vec3(cameraPos.x, cameraPos.y - height, cameraPos.z);
}

void Player::update()
{
}

void Player::draw(const ShaderProgram& program)
{
}
