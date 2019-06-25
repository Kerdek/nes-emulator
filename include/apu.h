#pragma once

#include <cstdint>

namespace nes
{
  class apu
  {
    apu(apu const &) = delete;
    apu(apu &&) = delete;
    apu & operator=(apu const &) = delete;
    apu & operator=(apu &&) = delete;
  public:
    apu();
    void reset();

    uint8_t read(int);
    void write(int, uint16_t, uint8_t);

    void run_frame(int);
  };
}
