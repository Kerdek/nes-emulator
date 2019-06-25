#pragma once

#include "mapper.h"

#include <vector>

namespace nes
{
  class mapper0 : public mapper
  {
  public:
    mapper0(
        nes::ppu & ppu,
        nes::cartridge_info const & info,
        std::vector<uint8_t> && prg,
        std::vector<uint8_t> && chr);

    void reset() override;
  };
}
