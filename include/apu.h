#pragma once

#include "bus.h"
#include "types.h"

namespace nes {
class apu {
public:
  void set_bus(nes::bus&);

  void power_on();

  uint8_t read(const int);
  void    write(const int, uint16_t, uint8_t);

  void run_frame(int);

private:
  nes::bus* bus = nullptr;
};
}  // namespace nes