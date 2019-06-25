#include "mappers/mapper2.h"
#include "ppu.h"

namespace nes
{
  mapper2::mapper2(
      nes::ppu & ppu,
      nes::cartridge_info const & info,
      std::vector<uint8_t> && prg,
      std::vector<uint8_t> && chr)
    : mapper(ppu, info, std::move(prg), std::move(chr))
  { }

  void mapper2::reset()
  {
    set_prg_map<16>(0, 0);
    set_prg_map<16>(1, -1);
    set_chr_map<8>(0, 0);

    ppu.set_mirroring(info.mirroring);
  }

  void mapper2::prg_write(uint16_t addr, uint8_t value)
  {
    if (addr >= 0x8000) set_prg_map<16>(0, value);
    else throw std::runtime_error{ "Illegal write." };
  }

  void mapper2::chr_write(uint16_t addr, uint8_t value)
  {
    chr[addr] = value;
  }
}
