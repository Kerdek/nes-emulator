#pragma once

#include "display.h"
#include "input.h"
#include "system.h"

namespace PPU
{
  template<Scanline s> void scanline_cycle();
}
namespace platform
{
  class subsystem
  {
    template<PPU::Scanline s> friend void PPU::scanline_cycle();

    sdl2::sdl2            sdl2;
    platform::input       input;
    platform::display     display;
    nes::system           system;

    subsystem(subsystem const &) = delete;
    subsystem(subsystem &&) = delete;
    subsystem & operator=(subsystem const &) = delete;
    subsystem & operator=(subsystem &&) = delete;

  public:
    subsystem(std::filesystem::path const & rom);
  };
}
