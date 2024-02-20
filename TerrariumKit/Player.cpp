#include "Player.h"

#include "Input.h"
#include "Keybindings.h"
#include "Time.h"

Player::Player(ICamera* camera, float walkSpeed)
	: camera_{ camera }, walkSpeed_{ walkSpeed }, velocity_{ 0.0f }
{

}

Player::~Player()
{

}

void Player::update()
{
	float xRel = SysTK::Input::getMouseAxis(SysTK::MouseAxis::X_AXIS);
	float yRel = SysTK::Input::getMouseAxis(SysTK::MouseAxis::Y_AXIS);
	camera_->transform().rotate(xRel, -yRel);

	float yWheel = SysTK::Input::getMouseWheel();
	camera_->zoom(yWheel);

	velocity_ = glm::vec3{ 0.0f };
	double deltaTime = SysTK::Time::deltaTime();
	float speed = static_cast<float>(walkSpeed_ * deltaTime);

	if (SysTK::Input::getKey(SysTK::Keybindings::upKey))
	{
		velocity_ += camera_->transform().front() * speed;
	}
	if (SysTK::Input::getKey(SysTK::Keybindings::downKey))
	{
		velocity_ += -camera_->transform().front() * speed;
	}
	if (SysTK::Input::getKey(SysTK::Keybindings::leftKey))
	{
		velocity_ += -camera_->transform().right() * speed;
	}
	if (SysTK::Input::getKey(SysTK::Keybindings::rightKey))
	{
		velocity_ += camera_->transform().right() * speed;
	}

	camera_->transform().translate(velocity_);
}

void Player::draw(const ShaderProgram& program)
{

}
