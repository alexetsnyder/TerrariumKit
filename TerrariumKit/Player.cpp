#include "Player.h"

Player::Player(ICamera* camera)
	: _camera{ camera }, _position{ camera->position() }
{
	_position.y -= height;
}

Player::~Player()
{
}

void Player::move(PlayerDirection direction, double deltaTime)
{
	CameraDirection cameraDir = CameraDirectionFromPlayerDirection(direction);
	_camera->move(cameraDir, deltaTime);

	glm::vec3 cameraPos = _camera->position();
	_position = glm::vec3(cameraPos.x, cameraPos.y - height, cameraPos.z);
}

void Player::update()
{
}

void Player::draw(const ShaderProgram& program)
{
}

CameraDirection Player::CameraDirectionFromPlayerDirection(PlayerDirection direction)
{
	CameraDirection cameraDir;

	switch (direction)
	{
		case PlayerDirection::FORWARD:
			cameraDir = CameraDirection::FORWARD;
			break;
		case PlayerDirection::BACKWARD:
			cameraDir = CameraDirection::BACKWARD;
			break;
		case PlayerDirection::LEFT:
			cameraDir = CameraDirection::LEFT;
			break;
		case PlayerDirection::RIGHT:
			cameraDir = CameraDirection::RIGHT;
			break;
	}

	return cameraDir;
}
