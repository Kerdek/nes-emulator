#pragma once

#include "mapper.h"

#include <array>
#include <vector>

namespace nes
{
  class mapper1 : public mapper
  {
    void apply();

    int     write_counter;
    uint8_t shift_reg;

    uint8_t control;
    uint8_t chr_bank_0;
    uint8_t chr_bank_1;
    uint8_t prg_bank;

  public:
    mapper1(
        nes::ppu & ppu,
        nes::cartridge_info && info,
        std::vector<uint8_t> && prg,
        std::vector<uint8_t> && chr);

    void reset() override;

    void prg_write(const uint16_t, const uint8_t) override;
    void chr_write(const uint16_t, const uint8_t) override;
  };
}
