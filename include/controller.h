#pragma once

#include <array>

#include "bus.h"
#include "types.h"

namespace nes {
class controller {
public:
  void set_bus(nes::bus&);

  uint8_t read(const size_t);
  void    write(const bool);

private:
  nes::bus* bus = nullptr;

  bool                   strobe = false;   // strobe latch
  std::array<uint8_t, 2> controller_bits{};  // shift registers
};
}  // namespace nes