#include "cartridge.h"
#include "log.h"
#include "mappers/mapper0.h"
#include "mappers/mapper1.h"
#include "mappers/mapper2.h"

#include <array>
#include <fstream>
#include <stdexcept>
#include <vector>

namespace nes
{
  cartridge::cartridge(nes::ppu & ppu) :
    ppu{ ppu }
  { }

  void cartridge::load(const std::filesystem::path & rom_file)
  {
    using namespace mirroring;

    std::ifstream rom(rom_file, std::ios::binary);
    if (rom.fail()) throw std::runtime_error(std::string{ "Failed to open the ROM \"" } + std::filesystem::weakly_canonical(rom_file).c_str() + "\"");

    std::array<uint8_t, 16> header;
    rom.read(reinterpret_cast<char*>(header.data()), 16);

    info.rom_size     = std::filesystem::file_size(rom_file);
    info.mapper_num   = (header[7] & 0xF0) | (header[6] >> 4);
    info.prg_banks    = header[4];
    info.chr_banks    = header[5];
    info.chr_ram      = (info.chr_banks == 0);
    info.prg_ram_size = header[8] ? header[8] * 0x2000 : 0x2000;
    info.mirroring    = (header[6] & 1) ? Vertical : Horizontal;

    LOG(platform::log::Info) << "16KB PRG-ROM banks: " << info.prg_banks;
    LOG(platform::log::Info) << "8KB CHR-ROM banks: " << info.chr_banks;
    LOG(platform::log::Info) << "Name table mirroring: " << +(header[6] & 0xB);
    LOG(platform::log::Info) << "Mirroring: "
                   << (info.mirroring ? "Vertical" : "Horizontal");
    LOG(platform::log::Info) << "Mapper #: " << info.mapper_num;
    LOG(platform::log::Info) << "PRG RAM size: " << info.prg_ram_size;

    std::vector<uint8_t> prg(info.prg_banks * ct::prg_bank_size);
    std::vector<uint8_t> chr(info.chr_banks * ct::chr_bank_size);

    rom.read(reinterpret_cast<char*>(prg.data()), prg.size());
    rom.read(reinterpret_cast<char*>(chr.data()), chr.size());

    rom.close();

    switch (info.mapper_num)
    {
      case 0: mapper = std::make_unique<nes::mapper0>(ppu, std::move(info), std::move(prg), std::move(chr)); break;
      case 1: mapper = std::make_unique<nes::mapper1>(ppu, std::move(info), std::move(prg), std::move(chr)); break;
      case 2: mapper = std::make_unique<nes::mapper2>(ppu, std::move(info), std::move(prg), std::move(chr)); break;
      default: throw std::runtime_error("Mapper not implemented");
    }
    mapper->reset();
  }

  uint8_t cartridge::prg_read(const uint16_t addr) const
  {
    return mapper->prg_read(addr);
  }

  void cartridge::prg_write(const uint16_t addr, const uint8_t value)
  {
    mapper->prg_write(addr, value);
  }

  uint8_t cartridge::chr_read(const uint16_t addr) const
  {
    return mapper->chr_read(addr);
  }

  void cartridge::chr_write(const uint16_t addr, const uint8_t value)
  {
    mapper->chr_write(addr, value);
  }
}
