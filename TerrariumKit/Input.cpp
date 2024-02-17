#include "Input.h"

namespace SysTK
{
	float Input::xRel_;
	float Input::yRel_;
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
				retValue = xRel_;
				break;
			case MouseAxis::Y_AXIS:
				retValue = yRel_;
				break;
		}

		return retValue;
	}

	void Input::update()
	{
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