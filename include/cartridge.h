#pragma once

#include "mapper.h"

#include <filesystem>
#include <memory>

namespace nes
{
  class cartridge
  {
    nes::ppu & ppu;

    nes::cartridge_info          info{};
    std::unique_ptr<nes::mapper> mapper = nullptr;

    cartridge(cartridge const &) = delete;
    cartridge(cartridge &&) = delete;
    cartridge & operator=(apu const &) = delete;
    cartridge & operator=(apu &&) = delete;

  public:
    cartridge(nes::ppu & ppu);

    void load(const std::filesystem::path&);

    uint8_t prg_read(const uint16_t) const;
    uint8_t chr_read(const uint16_t) const;

    void prg_write(const uint16_t, const uint8_t);
    void chr_write(const uint16_t, const uint8_t);
  };
}
