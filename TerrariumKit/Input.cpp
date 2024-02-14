#include "Input.h"

namespace SysTK
{
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
}