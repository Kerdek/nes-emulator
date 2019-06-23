#pragma once

#include <array>
#include <vector>

#include "bus.h"
#include "types.h"

namespace nes {
class mapper {
public:
  mapper(
      const nes::cartridge_info&,
      const std::vector<uint8_t>&,
      const std::vector<uint8_t>&);
  virtual ~mapper() = default;

  virtual void reset() = 0;

  void set_bus(nes::bus&);

  uint8_t prg_read(const uint16_t) const;
  uint8_t chr_read(const uint16_t) const;

  virtual void prg_write(const uint16_t, const uint8_t);
  virtual void chr_write(const uint16_t, const uint8_t);

  template <auto> void set_prg_map(int, int);
  template <auto> void set_chr_map(int, int);

protected:
  void set_mirroring(const int);

  const nes::cartridge_info& info;

  std::vector<uint8_t> prg;
  std::vector<uint8_t> prg_ram;
  std::vector<uint8_t> chr;

  std::array<size_t, 4> prg_map{};
  std::array<size_t, 8> chr_map{};

  nes::bus* bus = nullptr;
};
}  // namespace nes
