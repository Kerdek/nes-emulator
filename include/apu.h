#pragma once

#include "types.h"

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

    uint8_t read(const int);
    void write(const int, uint16_t, uint8_t);

    void run_frame(int);
  };
}
