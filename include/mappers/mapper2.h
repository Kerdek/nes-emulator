#pragma once

#include <array>
#include <vector>

#include "mapper.h"
#include "types.h"

namespace nes {
class mapper2 : public mapper {
public:
  mapper2(
      const nes::cartridge_info&,
      const std::vector<uint8_t>&,
      const std::vector<uint8_t>&);

  void reset() override;

  void prg_write(const uint16_t, const uint8_t) override;
  void chr_write(const uint16_t, const uint8_t) override;
};
}  // namespace nes