#include "Input.h"

namespace SysTK
{
	float Input::mouseSensitivity_{ 0.1f };
	float Input::xRel_{ 0.0f };
	float Input::yRel_{ 0.0f };
	float Input::yWheel_{ 0.0f };
	std::unordered_map<SDL_Keycode, bool> Input::keys_{};

	bool Input::getKey(SDL_Keycode key)
	{
		auto it = keys_.find(key);
		if (it != keys_.end())
		{
			return it->second;
		}
		
		return false;
	}

	float Input::getMouseAxis(MouseAxis axis)
	{
		float retValue{ 0.0f };

		switch (axis)
		{
			case MouseAxis::X_AXIS:
				retValue = mouseSensitivity_ * xRel_;
				break;
			case MouseAxis::Y_AXIS:
				retValue = mouseSensitivity_ * yRel_;
				break;
		}

		return retValue;
	}

	float Input::getMouseWheel()
	{
		return yWheel_;
	}

	void Input::processInput(SDL_Event event)
	{
		switch (event.type)
		{
			case SDL_KEYDOWN:
				addKey(event.key.keysym.sym);
				break;
			case SDL_KEYUP:
				removeKey(event.key.keysym.sym);
				break;
			case SDL_MOUSEMOTION:
				processMouseMotion(event);
				break;
			case SDL_MOUSEWHEEL:
				yWheel_ = static_cast<float>(event.wheel.y);
				break;
		}
	}

	void Input::reset()
	{
		yWheel_ = 0.0f;
		xRel_ = 0.0f;
		yRel_ = 0.0f;
	}

	void Input::addKey(SDL_Keycode key)
	{
		if (keys_.find(key) == keys_.end())
		{
			keys_.emplace(key, true);
		}
	}

	void Input::removeKey(SDL_Keycode key)
	{
		auto it = keys_.find(key);
		if (it != keys_.end())
		{
			keys_.erase(it);
		}
	}

	void Input::processMouseMotion(SDL_Event event)
	{
		xRel_ = static_cast<float>(event.motion.xrel);
		yRel_ = static_cast<float>(event.motion.yrel);
	}
}