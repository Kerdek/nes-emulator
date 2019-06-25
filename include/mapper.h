#pragma once

#include <array>
#include <vector>

#include "types.h"

namespace nes
{
  class mapper
  {
    mapper(mapper const &) = delete;
    mapper(mapper &&) = delete;
    mapper & operator=(mapper const &) = delete;
    mapper & operator=(mapper &&) = delete;

  protected:
    void set_mirroring(const int);

    nes::ppu & ppu;

    nes::cartridge_info const & info;
    std::vector<uint8_t> prg;
    std::vector<uint8_t> chr;

    std::vector<uint8_t> prg_ram;

    std::array<size_t, 4> prg_map{};
    std::array<size_t, 8> chr_map{};

  public:
    mapper(
        nes::ppu & ppu,
        nes::cartridge_info && info,
        std::vector<uint8_t> && prg,
        std::vector<uint8_t> && chr);

    virtual ~mapper() = default;

    virtual void reset() = 0;

    virtual void prg_write(const uint16_t, const uint8_t);
    virtual void chr_write(const uint16_t, const uint8_t);

    uint8_t prg_read(const uint16_t) const;
    uint8_t chr_read(const uint16_t) const;

    template <auto size> void set_prg_map(int, int);
    template <auto size> void set_chr_map(int, int);
  };
}
