#include "system.h"

namespace nes
{
	system::system(std::filesystem::path const & rom, platform::input & input, platform::display & display) :
		ram{},
		ppu{ *this, display },
		apu{},
		controller{ input },
		cartridge{ ppu },
		memory_mapper{ ram, ppu, apu, controller, cartridge },
		cpu{ ppu, memory_mapper },
		debugger{ memory_mapper, cpu }
	{
		cartridge.load(rom);
		ram.reset();
		cpu.reset();
		ppu.reset();
		apu.reset();
	}

	void system::run_frame()
	{
		cpu.run_frame();
	}
}
