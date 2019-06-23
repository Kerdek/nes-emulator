#pragma once

#include "types.h"

namespace nes {
class cpu;
class apu;
class ppu;
class cartridge;
class controller;
class debugger;
class emulator;

class bus {
public:
  void set_component(nes::cpu&);
  void set_component(nes::ppu&);
  void set_component(nes::apu&);
  void set_component(nes::cartridge&);
  void set_component(nes::controller&);
  void set_component(nes::debugger&);
  void set_component(nes::emulator&);

  //
  // CPU access
  //

  void run_frame();
  void set_nmi(const bool = true);
  void set_irq(const bool = true);

  //
  // PPU access
  //

  void    ppu_step();
  uint8_t ppu_read(const uint16_t);
  void    ppu_write(const uint16_t, const uint8_t);
  void    set_mirroring(const int);

  //
  // APU access
  //
  uint8_t apu_read(const int);
  void    apu_write(const int, const uint16_t, const uint8_t);

  //
  // Cartridge access
  //

  uint8_t prg_read(const uint16_t) const;
  void    prg_write(const uint16_t, const uint8_t);

  uint8_t chr_read(const uint16_t) const;
  void    chr_write(const uint16_t, const uint8_t);

  //
  // Controller access
  //

  uint8_t controller_read(size_t) const;
  void    controller_write(bool);

  //
  // Debugger access
  //

  void nestest();

  //
  // Emulator access
  //

  void    update_frame(const uint32_t*);
  uint8_t get_controller(const size_t) const;

private:
  nes::cpu*        cpu        = nullptr;
  nes::ppu*        ppu        = nullptr;
  nes::apu*        apu        = nullptr;
  nes::cartridge*  cartridge  = nullptr;
  nes::controller* controller = nullptr;
  nes::debugger*   debugger   = nullptr;
  nes::emulator*   emulator   = nullptr;
};
}  // namespace nes