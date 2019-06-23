#include "apu.h"

namespace nes {
void apu::set_bus(nes::bus& ref)
{
  this->bus = &ref;
}

void apu::power_on() {}

uint8_t apu::read(const int)
{
  return 0;
}

void apu::write(const int, const uint16_t, const uint8_t) {}

void apu::run_frame(int) {}
}  // namespace nes