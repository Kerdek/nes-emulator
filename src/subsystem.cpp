#include "subsystem.h"

#include "sdlfx.h"

namespace platform
{
	subsystem::subsystem(std::filesystem::path const & rom) :
		sdl2{},
		input{},
		display{},
		system{ rom, input, display }
	{
		for (;;)
		{
			for (SDL_Event e; SDL_PollEvent(&e);)
			{
				switch (e.type)
				{
				case SDL_QUIT:
					return;
				}
			}

			// this->bus->nestest();
			system.run_frame();
			subsystem::display.draw();
		}
	}
}
