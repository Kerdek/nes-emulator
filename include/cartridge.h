#pragma once

#include <filesystem>
#include <memory>

#include "bus.h"
#include "mapper.h"
#include "types.h"

namespace nes {
class cartridge {
public:
  void set_bus(nes::bus&);

  void load(const std::filesystem::path&);

  uint8_t prg_read(const uint16_t) const;
  uint8_t chr_read(const uint16_t) const;

  void prg_write(const uint16_t, const uint8_t);
  void chr_write(const uint16_t, const uint8_t);

private:
  nes::bus*                    bus = nullptr;
  nes::cartridge_info          info{};
  std::unique_ptr<nes::mapper> mapper = nullptr;
};
}  // namespace nes