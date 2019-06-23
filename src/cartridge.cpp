#include "cartridge.h"

#include <array>
#include <fstream>
#include <stdexcept>
#include <vector>

#include "log.h"
#include "mappers/mapper0.h"
#include "mappers/mapper1.h"
#include "mappers/mapper2.h"

namespace nes {
void cartridge::set_bus(nes::bus& ref)
{
  this->bus = &ref;
}

void cartridge::load(const std::filesystem::path& rom_file)
{
  using namespace mirroring;

  std::ifstream rom(rom_file, std::ios::binary);

  if (!rom) {
    throw std::runtime_error("Can't open the ROM");
  }

  std::array<uint8_t, 16> header;
  rom.read(reinterpret_cast<char*>(header.data()), 16);

  info.rom_size     = std::filesystem::file_size(rom_file);
  info.mapper_num   = (header[7] & 0xF0) | (header[6] >> 4);
  info.prg_banks    = header[4];
  info.chr_banks    = header[5];
  info.chr_ram      = (info.chr_banks == 0);
  info.prg_ram_size = header[8] ? header[8] * 0x2000 : 0x2000;
  info.mirroring    = (header[6] & 1) ? Vertical : Horizontal;

  LOG(log::Info) << "16KB PRG-ROM banks: " << info.prg_banks;
  LOG(log::Info) << "8KB CHR-ROM banks: " << info.chr_banks;
  LOG(log::Info) << "Name table mirroring: " << +(header[6] & 0xB);
  LOG(log::Info) << "Mirroring: "
                 << (info.mirroring ? "Vertical" : "Horizontal");
  LOG(log::Info) << "Mapper #: " << info.mapper_num;
  LOG(log::Info) << "PRG RAM size: " << info.prg_ram_size;

  std::vector<uint8_t> prg(info.prg_banks * ct::prg_bank_size);
  std::vector<uint8_t> chr(info.chr_banks * ct::chr_bank_size);

  rom.read(reinterpret_cast<char*>(prg.data()), prg.size());
  rom.read(reinterpret_cast<char*>(chr.data()), chr.size());

  rom.close();

  switch (info.mapper_num) {
    case 0:
      mapper = std::make_unique<nes::mapper0>(
          this->info, std::move(prg), std::move(chr));
      break;
    case 1:
      mapper = std::make_unique<nes::mapper1>(
          this->info, std::move(prg), std::move(chr));
      break;
    case 2:
      mapper = std::make_unique<nes::mapper2>(
          this->info, std::move(prg), std::move(chr));
      break;
    default: throw std::runtime_error("Mapper not implemented");
  }

  mapper->set_bus(*this->bus);
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
}  // namespace nes