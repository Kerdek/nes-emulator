#include "system.h"

namespace nes
{
	system::system(std::filesystem::path const & rom, platform::input & input, platform::display & display) :
		nmi_flipflop{},
		ram{},
		ppu{ cartridge, display, nmi_flipflop },
		apu{},
		controller{ input },
		cartridge{ ppu },
		memory_mapper{ ram, ppu, apu, controller, cartridge },
		cpu{ memory_mapper, nmi_flipflop, ppu },
		debugger{ memory_mapper, cpu }
	{
		cartridge.load(rom);
		ram.reset();
		ppu.reset();
		apu.reset();
		cpu.reset();
	}

	void system::run_frame()
	{
		cpu.run_frame();
	}
}
