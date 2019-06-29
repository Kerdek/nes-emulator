#include "system.h"

namespace nes
{
	system::system(std::filesystem::path const & rom, platform::input & input, platform::display & display) :
	  ppu{ *this, display },
	  cartridge{ ppu },
	  controller{ input },
	  apu{},
	  cpu{ ppu, apu, controller, cartridge },
	  debugger{ cpu }
	{
		cartridge.load(rom);
		cpu.reset();
		ppu.reset();
		apu.reset();
	}

	void system::run_frame()
	{
		cpu.run_frame();
	}
}
