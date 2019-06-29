#pragma once

#include "sdl.h"

#include <cstdint>

namespace platform
{
	class display
	{
		int const width  = 256;
		int const height = 240;

		sdl2::window   window;
		sdl2::renderer renderer;
		sdl2::texture  texture;

		display(display const &) = delete;
		display(display &&)		 = delete;
		display & operator=(display const &) = delete;
		display & operator=(display &&) = delete;

	  public:
		display();

		void update_frame(uint32_t const *);
		void draw();
		void close();
	};
}
