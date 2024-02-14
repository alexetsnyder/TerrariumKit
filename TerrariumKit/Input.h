#pragma once

#include <SDL/SDL.h>

#include <unordered_map>

namespace SysTK
{
	class Input
	{
		public:
			Input() = delete;
			Input(const Input&) = delete;

			static bool getKey(SDL_Keycode key);

			static void addKey(SDL_Keycode key);
			static void removeKey(SDL_Keycode key);

		private:
			static std::unordered_map<SDL_Keycode, bool> keys_;
	};
}


