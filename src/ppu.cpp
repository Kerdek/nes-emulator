#include "ppu.h"

#include "bus.h"
#include "types.h"

// Using LaiNES' PPU implementation
// while I don't implement my own
#include "ppu_lib.h"

namespace nes {
void ppu::set_bus(nes::bus& ref)
{
  PPU::bus = &ref;
}

void ppu::power_on()
{
  PPU::reset();
}

void ppu::reset()
{
  PPU::reset();
}

uint8_t ppu::read(const uint16_t addr)
{
  return PPU::access<0>(addr % 8);
}

void ppu::write(const uint16_t addr, const uint8_t value)
{
  PPU::access<1>(addr % 8, value);
}

void ppu::set_mirroring(const int mode)
{
  PPU::set_mirroring(mode);
}

void ppu::step()
{
  PPU::step();
}
}  // namespace nes