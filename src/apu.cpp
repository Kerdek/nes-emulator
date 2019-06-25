#include "apu.h"

namespace nes
{
  apu::apu()
  { }

  void apu::reset() {}

  uint8_t apu::read(const int)
  {
    return 0;
  }

  void apu::write(const int, const uint16_t, const uint8_t) {}

  void apu::run_frame(int) {}
}
