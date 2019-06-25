#include "mapper.h"
#include "ppu.h"
#include "log.h"

namespace nes
{
  mapper::mapper(
      nes::ppu & ppu,
      nes::cartridge_info const & info,
      std::vector<uint8_t> && prg,
      std::vector<uint8_t> && chr) :
    ppu{ ppu },
    info{ info },
    prg{ std::move(prg) },
    chr{ std::move(chr) }
  {
    if (mapper::info.chr_ram) mapper::chr.resize(0x2000, 0);
    prg_ram.resize(mapper::info.prg_ram_size, 0);
  }

  uint8_t mapper::prg_read(uint16_t addr) const
  {
    if (addr >= 0x8000) return prg[prg_map[(addr - 0x8000) / 0x2000] + (addr - 0x8000) % 0x2000];
    else return prg_ram[addr - 0x6000];
  }
  uint8_t mapper::chr_read(uint16_t addr) const
  {
    return chr[chr_map[addr / 0x400] + addr % 0x400];
  }
  void    mapper::prg_write(uint16_t, uint8_t)
  {
    LOG(platform::log::Error) << "Invalid write attempt. Writing isn't supported";
    throw std::runtime_error("Invalid write attempt. Writing isn't supported");
  }
  void    mapper::chr_write(uint16_t, uint8_t)
  {
    LOG(platform::log::Error) << "Invalid write attempt. Writing isn't supported";
    throw std::runtime_error("Invalid write attempt. Writing isn't supported");
  }

  // size must be in KBs
  template <auto size> void mapper::set_prg_map(int slot, int page)
  {
    constexpr size_t pages   = size / 8;
    constexpr size_t pages_b = size * 0x400;  // In bytes
    if (page < 0) page = (prg.size() / pages_b) + page;
    for (size_t i = 0; i < pages; ++i) prg_map[pages * slot + i] = ((pages_b * page) + 0x2000 * i) % prg.size();
  }
  template void mapper::set_prg_map<32>(int, int);
  template void mapper::set_prg_map<16>(int, int);
  template void mapper::set_prg_map<8>(int, int);

  template <auto size> void mapper::set_chr_map(int slot, int page)
  {
    for (size_t i = 0; i < size; ++i) chr_map[size * slot + i] = ((size * 0x400 * page) + 0x400 * i) % chr.size();
  }
  template void mapper::set_chr_map<8>(int, int);
  template void mapper::set_chr_map<4>(int, int);
  template void mapper::set_chr_map<2>(int, int);
  template void mapper::set_chr_map<1>(int, int);
}
