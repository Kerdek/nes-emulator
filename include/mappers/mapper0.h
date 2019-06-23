#pragma once

#include <vector>

#include "mapper.h"
#include "types.h"

namespace nes {
class mapper0 : public mapper {
public:
  mapper0(
      const nes::cartridge_info&,
      const std::vector<uint8_t>&,
      const std::vector<uint8_t>&);

  void reset() override;
};
}  // namespace nes