#pragma once

#include <cstddef>
#include <cstdint>

using std::int8_t;
using std::size_t;
using std::uint16_t;
using std::uint32_t;
using std::uint8_t;
using std::uintmax_t;

enum class OS { Windows, Linux, Unsupported };

#ifdef _WIN32
constexpr auto current_os = OS::Windows;
#elif __linux__
constexpr auto current_os = OS::Linux;
#else
constexpr auto current_os = OS::Unsupported;
#endif

#ifdef _DEBUG
constexpr auto debug_mode = true;
#else
constexpr auto debug_mode = false;
#endif

namespace nes {
namespace ct {
constexpr uint16_t prg_bank_size = 0x4000;  // 16384;
constexpr uint16_t chr_bank_size = 0x2000;  // 8192;
}  // namespace ct

//
// CPU
//

namespace interruption_type {
enum interruption_type { NMI, RST, IRQ, BRK };
}

namespace addressing_mode {
enum addressing_mode {
  Implicit,
  Accumulator,
  Immediate,
  ZeroPage,
  ZeroPageX,
  ZeroPageY,
  Relative,
  Absolute,
  AbsoluteX,
  AbsoluteX_Exception,
  AbsoluteY,
  AbsoluteY_Exception,
  Indirect,
  IndirectX,
  IndirectY,
  IndirectY_Exception,
  Invalid
};
}

namespace flags {
enum flags : uint8_t {
  Carry     = 0x01,
  Zero      = 0x02,
  Interrupt = 0x04,
  Decimal   = 0x08,
  Break     = 0x10,
  Reserved  = 0x20,
  Overflow  = 0x40,
  Negative  = 0x80
};
}

struct state {
  uint8_t  a  = 0;
  uint8_t  x  = 0;
  uint8_t  y  = 0;
  uint16_t pc = 0;
  uint8_t  sp = 0;
  uint8_t  sr = 0;

  uint8_t ps = 0;

  bool nmi_flag = false;
  bool irq_flag = false;

  int cycle_count = 0;

  bool check_flags(const uint8_t) const;
  void set_flags(const uint8_t);
  void clear_flags(const uint8_t);
  void update_nz(const uint8_t);

  void set_a(const uint8_t);
  void set_x(const uint8_t);
  void set_y(const uint8_t);
  void set_pc(const uint16_t);
  void set_ps(const uint8_t);
};

//
// CPU and PPU (memory)
//

namespace memory {
enum operation { None = -1, Read, Write };

enum cpu_map {
  Unknown = -1,
  CPU_RAM,
  PPU_Access,
  APU_Access,
  OAMDMA,
  Controller,
  Controller_1,
  Controller_2,
  Cartridge
};

enum ppu_map {
  PPUCTRL = 0x2000,
  PPUMASK = 0x2001,
  PPUSTATUS = 0x2002,
  OAMADDR = 0x2003,
  OAMDATA = 0x2004,
  PPUSCROLL = 0x2005,
  PPUADDR = 0x2006,
  PPUDATA = 0x2007
};

template <auto Operation> int get_cpu_map(const uint16_t);
template <auto Operation> int get_ppu_map(const uint16_t);
}  // namespace memory

//
// Cartridge
//

namespace mirroring {
enum mirroring { Unset = -1, Horizontal, Vertical };
}

struct cartridge_info {
  size_t rom_size;
  size_t mapper_num;
  size_t prg_banks;
  size_t chr_banks;
  size_t prg_ram_size;
  int    mirroring = mirroring::Unset;

  bool chr_ram = false;
};
}  // namespace nes