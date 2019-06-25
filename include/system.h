#pragma once

#include "cartridge.h"
#include "controller.h"
#include "ppu.h"
#include "apu.h"
#include "cpu.h"
#include "debugger.h"

namespace PPU
{
  enum Scanline { VISIBLE, POST, NMI, PRE };

  uint8_t rd(uint16_t addr);
  void wr(uint16_t addr, uint8_t v);
  template<Scanline s> void scanline_cycle();
}

namespace nes
{
  class system
  {
    friend uint8_t PPU::rd(uint16_t addr);
    friend void PPU::wr(uint16_t addr, uint8_t v);
    template<PPU::Scanline s> friend void PPU::scanline_cycle();

    nes::cartridge        cartridge;
    nes::controller       controller;
    nes::ppu              ppu;
    nes::apu              apu;
    nes::cpu              cpu;
    nes::debugger         debugger;

    system(system const &) = delete;
    system(system &&) = delete;
    system & operator=(system const &) = delete;
    system & operator=(system &&) = delete;

  public:
    system(std::filesystem::path const & rom, platform::input & input, platform::display & display);
    void run_frame();
  };
}
