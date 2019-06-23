#pragma once

#include <array>
#include <vector>

#include "mapper.h"
#include "types.h"

namespace nes {
class mapper1 : public mapper {
public:
  mapper1(
      const nes::cartridge_info&,
      const std::vector<uint8_t>&,
      const std::vector<uint8_t>&);

  void reset() override;

  void prg_write(const uint16_t, const uint8_t) override;
  void chr_write(const uint16_t, const uint8_t) override;

private:
  void apply();

  int     write_counter = 0;
  uint8_t shift_reg     = 0;

  uint8_t control = 0x0C;
  uint8_t chr_bank_0 = 0;
  uint8_t chr_bank_1 = 0;
  uint8_t prg_bank = 0;
};
}  // namespace nes