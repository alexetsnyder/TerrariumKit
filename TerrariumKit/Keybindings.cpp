#include "Keybindings.h"

namespace SysTK
{
	SDL_Keycode Keybindings::jumpKey{ SDLK_SPACE };

	SDL_Keycode Keybindings::upKey{ SDLK_w };
	SDL_Keycode Keybindings::downKey{ SDLK_s };
	SDL_Keycode Keybindings::leftKey{ SDLK_a };
	SDL_Keycode Keybindings::rightKey{ SDLK_d };

	SDL_Keycode Keybindings::runKey{ SDLK_LSHIFT };
}