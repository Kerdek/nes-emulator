#include "ppu.h"

#include "bus.h"
#include "types.h"

namespace nes {
void ppu::set_bus(nes::bus&) {}

void ppu::power_on() {}

void ppu::reset() {}

uint8_t ppu::read(const uint16_t)
{
  return 0;
}

void ppu::write(const uint16_t, const uint8_t) {}

void ppu::set_mirroring(const int) {}

void ppu::step() {}
}  // namespace nes