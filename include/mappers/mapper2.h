#pragma once

#include "mapper.h"

#include <array>
#include <vector>

namespace nes
{
  class mapper2 : public mapper
  {
  public:
    mapper2(
        nes::ppu & ppu,
        nes::cartridge_info const & info,
        std::vector<uint8_t> && prg,
        std::vector<uint8_t> && chr);

    void reset() override;
    void prg_write(uint16_t, uint8_t) override;
    void chr_write(uint16_t, uint8_t) override;
  };
}
