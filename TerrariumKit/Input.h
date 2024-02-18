#pragma once

#include <SDL/SDL.h>

#include <unordered_map>

namespace SysTK
{
	enum class MouseAxis
	{
		X_AXIS,
		Y_AXIS,
	};

	class Input
	{
		public:
			Input() = delete;
			Input(const Input&) = delete;

			static bool getKey(SDL_Keycode key);
			static float getMouseAxis(MouseAxis axis);
			static float getMouseWheel();

			static void processInput(SDL_Event event);
			static void update();

			static void addKey(SDL_Keycode key);
			static void removeKey(SDL_Keycode key);
			static void processMouseMotion(SDL_Event event);

		private:
			static float xRel_;
			static float yRel_;
			static float yWheel_;
			static std::unordered_map<SDL_Keycode, bool> keys_;
	};
}


