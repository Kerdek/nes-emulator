#pragma once

#include "bus.h"
#include "types.h"

namespace nes {
class ppu {
public:
  void set_bus(nes::bus&);
  void power_on();
  void reset();

  uint8_t read(const uint16_t);
  void    write(const uint16_t, const uint8_t);

  void set_mirroring(const int);

  void step();
};
}  // namespace nes