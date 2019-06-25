#include "apu.h"

namespace nes
{
  apu::apu()
  { }

  void apu::reset()
  { }

  uint8_t apu::read(int)
  {
    return 0;
  }

  void apu::write(int, uint16_t, uint8_t)
  { }

  void apu::run_frame(int)
  { }
}
