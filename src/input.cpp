#include "input.h"

#include "sdlfx.h"

constexpr SDL_Scancode KEY_A[2]		 = { SDL_SCANCODE_A, SDL_SCANCODE_ESCAPE };
constexpr SDL_Scancode KEY_B[2]		 = { SDL_SCANCODE_S, SDL_SCANCODE_ESCAPE };
constexpr SDL_Scancode KEY_SELECT[2] = { SDL_SCANCODE_BACKSPACE, SDL_SCANCODE_ESCAPE };
constexpr SDL_Scancode KEY_START[2]  = { SDL_SCANCODE_RETURN, SDL_SCANCODE_ESCAPE };
constexpr SDL_Scancode KEY_UP[2]	 = { SDL_SCANCODE_UP, SDL_SCANCODE_ESCAPE };
constexpr SDL_Scancode KEY_DOWN[2]   = { SDL_SCANCODE_DOWN, SDL_SCANCODE_ESCAPE };
constexpr SDL_Scancode KEY_LEFT[2]   = { SDL_SCANCODE_LEFT, SDL_SCANCODE_ESCAPE };
constexpr SDL_Scancode KEY_RIGHT[2]  = { SDL_SCANCODE_RIGHT, SDL_SCANCODE_ESCAPE };

namespace platform
{
	input::input()
	{
		keys = SDL_GetKeyboardState(nullptr);
	}
	uint8_t input::get_controller(int n) const
	{
		return (keys[KEY_A[n]]) << 0
			   | (keys[KEY_B[n]]) << 1
			   | (keys[KEY_SELECT[n]]) << 2
			   | (keys[KEY_START[n]]) << 3
			   | (keys[KEY_UP[n]]) << 4
			   | (keys[KEY_DOWN[n]]) << 5
			   | (keys[KEY_LEFT[n]]) << 6
			   | (keys[KEY_RIGHT[n]]) << 7;
	}
}
