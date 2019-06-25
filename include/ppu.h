#pragma once

#include "types.h"

namespace nes
{
  class ppu
  {
    ppu(ppu const &) = delete;
    ppu(ppu &&) = delete;
    ppu & operator=(ppu const &) = delete;
    ppu & operator=(ppu &&) = delete;

  public:
    ppu(nes::system & system, platform::display & display);
    void reset();

    uint8_t read(const uint16_t);
    void write(const uint16_t, const uint8_t);

    void set_mirroring(const int);

    void step();
  };
}
