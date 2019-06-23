#include "mapper.h"

#include "log.h"

namespace nes {
mapper::mapper(
    const nes::cartridge_info&  info_in,
    const std::vector<uint8_t>& prg_in,
    const std::vector<uint8_t>& chr_in)
  : info(info_in), prg(prg_in), chr(chr_in)
{
  if (info.chr_ram) {
    this->chr.resize(0x2000, 0);
  }

  this->prg_ram.resize(info.prg_ram_size, 0);
}

void mapper::set_bus(nes::bus& ref)
{
  this->bus = &ref;
}

void mapper::set_mirroring(const int mode)
{
  this->bus->set_mirroring(mode);
}

uint8_t mapper::prg_read(const uint16_t addr) const
{
  if (addr >= 0x8000) {
    const size_t slot     = (addr - 0x8000) / 0x2000;
    const size_t prg_addr = (addr - 0x8000) % 0x2000;

    return prg[prg_map[slot] + prg_addr];
  } else {
    return prg_ram[addr - 0x6000];
  }
}

uint8_t mapper::chr_read(uint16_t addr) const
{
  const size_t slot     = addr / 0x400;
  const size_t chr_addr = addr % 0x400;

  return chr[chr_map[slot] + chr_addr];
}

void mapper::prg_write(uint16_t, uint8_t)
{
  LOG(log::Error) << "Invalid write attempt. Writing isn't supported";
  throw std::runtime_error("Invalid write attempt. Writing isn't supported");
}

void mapper::chr_write(uint16_t, uint8_t)
{
  LOG(log::Error) << "Invalid write attempt. Writing isn't supported";
  throw std::runtime_error("Invalid write attempt. Writing isn't supported");
}

// size must be in KBs
template <auto size> void mapper::set_prg_map(int slot, int page)
{
  constexpr size_t pages   = size / 8;
  constexpr size_t pages_b = size * 0x400;  // In bytes

  if (page < 0) {
    page = (prg.size() / pages_b) + page;
  }

  for (size_t i = 0; i < pages; ++i) {
    prg_map[pages * slot + i] = ((pages_b * page) + 0x2000 * i) % prg.size();
  }
}

template <auto size> void mapper::set_chr_map(int slot, int page)
{
  constexpr size_t pages   = size;
  constexpr size_t pages_b = size * 0x400;  // In bytes

  for (size_t i = 0; i < size; ++i) {
    chr_map[pages * slot + i] = ((pages_b * page) + 0x400 * i) % chr.size();
  }
}

template void mapper::set_prg_map<32>(int, int);
template void mapper::set_prg_map<16>(int, int);
template void mapper::set_prg_map<8>(int, int);

template void mapper::set_chr_map<8>(int, int);
template void mapper::set_chr_map<4>(int, int);
template void mapper::set_chr_map<2>(int, int);
template void mapper::set_chr_map<1>(int, int);
}  // namespace nes