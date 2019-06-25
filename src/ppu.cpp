#include "ppu.h"

// Using LaiNES' PPU implementation
// while I don't implement my own
#include "ppu_lib.h"

namespace nes
{
  ppu::ppu(nes::system & bus, platform::display & display)
  {
    PPU::bus = &bus;
    PPU::display = &display;
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
}
