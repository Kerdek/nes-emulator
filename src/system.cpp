#include "system.h"

namespace nes
{
    system::system(std::filesystem::path const & rom, platform::input & input, platform::display & display) :
        cartridge{},
        ram{},
        apu{},
        controller{ input },
        nmi_flipflop{},
        ppu{ cartridge, display, nmi_flipflop },
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
