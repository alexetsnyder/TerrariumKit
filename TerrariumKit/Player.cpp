#include "Player.h"

#include "Input.h"
#include "Keybindings.h"
#include "Time.h"

Player::Player(ICamera* camera, const ProcGenTK::IChunkMediator* mediator, float walkSpeed)
	: camera_{ camera }, chunkMediator_{ mediator }, walkSpeed_{ walkSpeed }, velocity_{ 0.0f }
{
	verticalVelocity_ = 0.0f;
}

Player::~Player()
{

}

glm::vec3 Player::position() const
{
	glm::vec3 position{ camera_->transform().position() };
	position.y -= cameraHeight;
	return position;
}

void Player::update()
{
	float xRel = SysTK::Input::getMouseAxis(SysTK::MouseAxis::X_AXIS);
	float yRel = SysTK::Input::getMouseAxis(SysTK::MouseAxis::Y_AXIS);
	camera_->transform().rotate(xRel, -yRel);

	float yWheel = SysTK::Input::getMouseWheel();
	camera_->zoom(yWheel);

	float deltaTime = static_cast<float>(SysTK::Time::deltaTime());
	calculateVelocity();
	camera_->transform().translate(velocity_ * deltaTime);
}

void Player::draw(const ShaderProgram& program)
{

}

void Player::calculateVelocity()
{
	velocity_ = glm::vec3{ 0.0f };

	if (verticalVelocity_ > gravity)
	{
		verticalVelocity_ += gravity * static_cast<float>(SysTK::Time::deltaTime());
	}

	velocity_ += verticalVelocity_ * glm::vec3(0.0f, 1.0f, 0.0f);

	float speed = walkSpeed_;
	if (SysTK::Input::getKey(SysTK::Keybindings::upKey))
	{
		glm::vec3 front = camera_->transform().front();
		velocity_ += glm::vec3(front.x, 0.0f, front.z) * speed;
	}
	if (SysTK::Input::getKey(SysTK::Keybindings::downKey))
	{
		glm::vec3 front = camera_->transform().front();
		velocity_ += -glm::vec3(front.x, 0.0f, front.z) * speed;
	}
	if (SysTK::Input::getKey(SysTK::Keybindings::leftKey))
	{
		velocity_ += -camera_->transform().right() * speed;
	}
	if (SysTK::Input::getKey(SysTK::Keybindings::rightKey))
	{
		velocity_ += camera_->transform().right() * speed;
	}

	checkForCollision();
}

void Player::checkForCollision()
{
	float deltaTime = static_cast<float>(SysTK::Time::deltaTime());
	if (velocity_.y > 0.0f && yCollision(height + velocity_.y * deltaTime))
	{
		velocity_.y = 0.0f;
	}
	else if (velocity_.y < 0.0f && yCollision(velocity_.y * deltaTime))
	{
		velocity_.y = 0.0f;
	}

	if (velocity_.x > 0.0f && xCollision(radius) || 
		velocity_.x < 0.0f && xCollision(-radius))
	{
		velocity_.x = 0.0f;
	}

	if (velocity_.z > 0.0f && zCollision(radius) ||
		velocity_.z < 0.0f && zCollision(-radius))
	{
		velocity_.z = 0.0f;
	}
}

bool Player::yCollision(float dy)
{
	return (
		   chunkMediator_->hasSolidVoxel(glm::vec3(position().x - radius, position().y + dy, position().z - radius))
		|| chunkMediator_->hasSolidVoxel(glm::vec3(position().x - radius, position().y + dy, position().z + radius))
		|| chunkMediator_->hasSolidVoxel(glm::vec3(position().x + radius, position().y + dy, position().z + radius))
		|| chunkMediator_->hasSolidVoxel(glm::vec3(position().x + radius, position().y + dy, position().z - radius))
	);
}

bool Player::xCollision(float dx)
{
	return (
		   chunkMediator_->hasSolidVoxel(glm::vec3(position().x + dx, position().y, position().z))
		|| chunkMediator_->hasSolidVoxel(glm::vec3(position().x + dx, position().y + height, position().z))
	);
}

bool Player::zCollision(float dz)
{
	return (
		   chunkMediator_->hasSolidVoxel(glm::vec3(position().x, position().y, position().z + dz))
		|| chunkMediator_->hasSolidVoxel(glm::vec3(position().x, position().y + height, position().z + dz))
	);
}
