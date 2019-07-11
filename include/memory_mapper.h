#pragma once

#include <cstdint>

namespace nes
{
    class ram;
    class apu;
    class ppu;
    class cartridge;
    class controller;

    class memory_mapper
    {
        nes::ram &        ram;
        nes::ppu &        ppu;
        nes::apu &        apu;
        nes::controller & controller;
        nes::cartridge &  cartridge;

    public:
        memory_mapper(nes::ram & ram, nes::ppu & ppu, nes::apu & apu, nes::controller & controller, nes::cartridge & cartridge);

        uint8_t read(uint16_t) const;
        void    write(uint16_t, uint8_t);
    };
}
