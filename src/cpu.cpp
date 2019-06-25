#include "cpu.h"
#include "cartridge.h"
#include "controller.h"
#include "ppu.h"
#include "apu.h"

#include <iostream>
#include <stdexcept>

namespace nes
{
  cpu::cpu(nes::ppu & ppu, nes::apu & apu, nes::controller & controller, nes::cartridge & cartridge) :
    ppu{ ppu },
    apu{ apu },
    controller{ controller },
    cartridge{ cartridge }
  { }

  void cpu::reset()
  {
    remaining_cycles = 0;
    ram.fill(0);
    state.set_ps(0x34);
    INT_RST();
  }

  void cpu::set_nmi(const bool value)
  {
    state.nmi_flag = value;
  }

  void cpu::set_irq(const bool value)
  {
    state.irq_flag = value;
  }

  void cpu::dma_oam(const uint8_t addr)
  {
    for (size_t i = 0; i < 256; ++i)
    {
      // 0x2014 == OAMDATA
      memory_write(0x2014, memory_read((addr * 0x100) + i));
    }
  }

  void cpu::run_frame()
  {
    remaining_cycles += total_cycles;

    while (remaining_cycles > 0)
    {
      if (state.nmi_flag) INT_NMI();
      else if (state.irq_flag && !state.check_flags(flags::Interrupt)) INT_IRQ();
      execute();
    }

    // state.cycle_count = 0;
  }

  void cpu::tick()
  {
    ppu.step();
    ppu.step();
    ppu.step();
    --remaining_cycles;

    // ++state.cycle_count;
  }

  uint8_t cpu::read(const uint16_t addr) const
  {
    using namespace memory;

    switch (get_cpu_map<Read>(addr))
    {
      case CPU_RAM: return ram[addr % 0x800];
      case PPU_Access: return ppu.read(addr);
      case APU_Access: return apu.read(elapsed());
      case Controller_1: return controller.read(0);
      case Controller_2: return controller.read(1);
      case Cartridge: return cartridge.prg_read(addr);
      default: throw std::runtime_error("Invalid read address");
    }
  }

  void cpu::write(const uint16_t addr, const uint8_t value)
  {
    using namespace memory;

    switch (get_cpu_map<Write>(addr))
    {
      case CPU_RAM: ram[addr % 0x800] = value; break;
      case PPU_Access: ppu.write(addr, value); break;
      case APU_Access: apu.write(elapsed(), addr, value); break;
      case OAMDMA: dma_oam(value); break;
      case Controller: controller.write(value & 1); break;
      case Cartridge: cartridge.prg_write(addr, value); break;
      default: throw std::runtime_error("Invalid write address");
    }
  }

  uint8_t cpu::memory_read(const uint16_t addr)
  {
    tick();
    return read(addr);
  }

  void cpu::memory_write(const uint16_t addr, const uint8_t value)
  {
    tick();
    write(addr, value);
  }

  uint8_t cpu::peek(const uint16_t addr) const
  {
    return read(addr);
  }

  uint16_t cpu::peek_imm() const
  {
    return state.pc + 1;
  }

  uint16_t cpu::peek_rel() const
  {
    return state.pc + 1;
  }

  uint16_t cpu::peek_zp() const
  {
    return peek(peek_imm());
  }

  uint16_t cpu::peek_zpx() const
  {
    return (peek_zp() + state.x) & 0xFF;
  }

  uint16_t cpu::peek_zpy() const
  {
    return (peek_zp() + state.y) & 0xFF;
  }

  uint16_t cpu::peek_ab() const
  {
    const auto base_addr = peek_imm();
    return (peek(base_addr + 1) << 8) | peek(base_addr);
  }

  uint16_t cpu::peek_abx() const
  {
    const auto base_addr = peek_ab();
    return base_addr + state.x;
  }

  uint16_t cpu::peek_aby() const
  {
    const auto base_addr = peek_ab();
    return base_addr + state.y;
  }

  uint16_t cpu::peek_ind() const
  {
    const auto base_addr = peek_ab();
    return peek(base_addr) |
           (peek((base_addr & 0xFF00) | ((base_addr + 1) % 0x100)) << 8);
  }

  uint16_t cpu::peek_indx() const
  {
    const auto base_addr = peek_zpx();
    return (peek((base_addr + 1) & 0xFF) << 8) | peek(base_addr);
  }

  uint16_t cpu::peek_indy() const
  {
    const auto base_addr = peek_zp();
    return ((peek((base_addr + 1) & 0xFF) << 8) | peek(base_addr)) + state.y;
  }

  int cpu::elapsed() const
  {
    return total_cycles - remaining_cycles;
  }
}
