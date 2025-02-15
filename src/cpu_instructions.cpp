#include "cpu.h"

#include <iostream>
#include <stdexcept>

#include "log.h"

using namespace nes::addressing_mode;

namespace nes {
template uint16_t    cpu::get_operand<Immediate>();
template uint16_t    cpu::get_operand<Relative>();
template <> uint16_t cpu::get_operand<ZeroPage>();
template <> uint16_t cpu::get_operand<ZeroPageX>();
template <> uint16_t cpu::get_operand<ZeroPageY>();
template <> uint16_t cpu::get_operand<Absolute>();
template <> uint16_t cpu::get_operand<AbsoluteX>();
template <> uint16_t cpu::get_operand<AbsoluteX_Exception>();
template <> uint16_t cpu::get_operand<AbsoluteY>();
template <> uint16_t cpu::get_operand<AbsoluteY_Exception>();
template <> uint16_t cpu::get_operand<Indirect>();
template <> uint16_t cpu::get_operand<IndirectX>();
template <> uint16_t cpu::get_operand<IndirectY>();
template <> uint16_t cpu::get_operand<IndirectY_Exception>();

void cpu::execute()
{
  const auto opcode = memory_read(get_operand<Immediate>());

  switch (opcode) {
    case 0x00: return INT_BRK();
    case 0x01: return ORA<IndirectX>();
    case 0x05: return ORA<ZeroPage>();
    case 0x06: return ASL<ZeroPage>();
    case 0x08: return PHP();
    case 0x09: return ORA<Immediate>();
    case 0x0A: return ASL<Accumulator>();
    case 0x0D: return ORA<Absolute>();
    case 0x0E: return ASL<Absolute>();
    case 0x10: return BPL();
    case 0x11: return ORA<IndirectY>();
    case 0x15: return ORA<ZeroPageX>();
    case 0x16: return ASL<ZeroPageX>();
    case 0x18: return CLC();
    case 0x19: return ORA<AbsoluteY>();
    case 0x1D: return ORA<AbsoluteX>();
    case 0x1E: return ASL<AbsoluteX_Exception>();
    case 0x20: return JSR();
    case 0x21: return AND<IndirectX>();
    case 0x24: return BIT<ZeroPage>();
    case 0x25: return AND<ZeroPage>();
    case 0x26: return ROL<ZeroPage>();
    case 0x28: return PLP();
    case 0x29: return AND<Immediate>();
    case 0x2A: return ROL<Accumulator>();
    case 0x2C: return BIT<Absolute>();
    case 0x2D: return AND<Absolute>();
    case 0x2E: return ROL<Absolute>();
    case 0x30: return BMI();
    case 0x31: return AND<IndirectY>();
    case 0x35: return AND<ZeroPageX>();
    case 0x36: return ROL<ZeroPageX>();
    case 0x38: return SEC();
    case 0x39: return AND<AbsoluteY>();
    case 0x3D: return AND<AbsoluteX>();
    case 0x3E: return ROL<AbsoluteX_Exception>();
    case 0x40: return RTI();
    case 0x41: return EOR<IndirectX>();
    case 0x45: return EOR<ZeroPage>();
    case 0x46: return LSR<ZeroPage>();
    case 0x48: return PHA();
    case 0x49: return EOR<Immediate>();
    case 0x4A: return LSR<Accumulator>();
    case 0x4C: return JMP<Absolute>();
    case 0x4D: return EOR<Absolute>();
    case 0x4E: return LSR<Absolute>();
    case 0x50: return BVC();
    case 0x51: return EOR<IndirectY>();
    case 0x55: return EOR<ZeroPageX>();
    case 0x56: return LSR<ZeroPageX>();
    case 0x58: return CLI();
    case 0x59: return EOR<AbsoluteY>();
    case 0x5D: return EOR<AbsoluteX>();
    case 0x5E: return LSR<AbsoluteX_Exception>();
    case 0x60: return RTS();
    case 0x61: return ADC<IndirectX>();
    case 0x65: return ADC<ZeroPage>();
    case 0x66: return ROR<ZeroPage>();
    case 0x68: return PLA();
    case 0x69: return ADC<Immediate>();
    case 0x6A: return ROR<Accumulator>();
    case 0x6C: return JMP<Indirect>();
    case 0x6D: return ADC<Absolute>();
    case 0x6E: return ROR<Absolute>();
    case 0x70: return BVS();
    case 0x71: return ADC<IndirectY>();
    case 0x75: return ADC<ZeroPageX>();
    case 0x76: return ROR<ZeroPageX>();
    case 0x78: return SEI();
    case 0x79: return ADC<AbsoluteY>();
    case 0x7D: return ADC<AbsoluteX>();
    case 0x7E: return ROR<AbsoluteX_Exception>();
    case 0x81: return STA<IndirectX>();
    case 0x84: return STY<ZeroPage>();
    case 0x85: return STA<ZeroPage>();
    case 0x86: return STX<ZeroPage>();
    case 0x88: return DEY();
    case 0x8A: return TXA();
    case 0x8C: return STY<Absolute>();
    case 0x8D: return STA<Absolute>();
    case 0x8E: return STX<Absolute>();
    case 0x90: return BCC();
    case 0x91: return STA<IndirectY_Exception>();
    case 0x94: return STY<ZeroPageX>();
    case 0x95: return STA<ZeroPageX>();
    case 0x96: return STX<ZeroPageY>();
    case 0x98: return TYA();
    case 0x99: return STA<AbsoluteY_Exception>();
    case 0x9A: return TXS();
    case 0x9D: return STA<AbsoluteX_Exception>();
    case 0xA0: return LDY<Immediate>();
    case 0xA1: return LDA<IndirectX>();
    case 0xA2: return LDX<Immediate>();
    case 0xA4: return LDY<ZeroPage>();
    case 0xA5: return LDA<ZeroPage>();
    case 0xA6: return LDX<ZeroPage>();
    case 0xA8: return TAY();
    case 0xA9: return LDA<Immediate>();
    case 0xAA: return TAX();
    case 0xAC: return LDY<Absolute>();
    case 0xAD: return LDA<Absolute>();
    case 0xAE: return LDX<Absolute>();
    case 0xB0: return BCS();
    case 0xB1: return LDA<IndirectY>();
    case 0xB4: return LDY<ZeroPageX>();
    case 0xB5: return LDA<ZeroPageX>();
    case 0xB6: return LDX<ZeroPageY>();
    case 0xB8: return CLV();
    case 0xB9: return LDA<AbsoluteY>();
    case 0xBA: return TSX();
    case 0xBC: return LDY<AbsoluteX>();
    case 0xBD: return LDA<AbsoluteX>();
    case 0xBE: return LDX<AbsoluteY>();
    case 0xC0: return CPY<Immediate>();
    case 0xC1: return CMP<IndirectX>();
    case 0xC4: return CPY<ZeroPage>();
    case 0xC5: return CMP<ZeroPage>();
    case 0xC6: return DEC<ZeroPage>();
    case 0xC8: return INY();
    case 0xC9: return CMP<Immediate>();
    case 0xCA: return DEX();
    case 0xCC: return CPY<Absolute>();
    case 0xCD: return CMP<Absolute>();
    case 0xCE: return DEC<Absolute>();
    case 0xD0: return BNE();
    case 0xD1: return CMP<IndirectY>();
    case 0xD5: return CMP<ZeroPageX>();
    case 0xD6: return DEC<ZeroPageX>();
    case 0xD8: return CLD();
    case 0xD9: return CMP<AbsoluteY>();
    case 0xDD: return CMP<AbsoluteX>();
    case 0xDE: return DEC<AbsoluteX_Exception>();
    case 0xE0: return CPX<Immediate>();
    case 0xE1: return SBC<IndirectX>();
    case 0xE4: return CPX<ZeroPage>();
    case 0xE5: return SBC<ZeroPage>();
    case 0xE6: return INC<ZeroPage>();
    case 0xE8: return INX();
    case 0xE9: return SBC<Immediate>();
    case 0xEA: return NOP();
    case 0xEC: return CPX<Absolute>();
    case 0xED: return SBC<Absolute>();
    case 0xEE: return INC<Absolute>();
    case 0xF0: return BEQ();
    case 0xF1: return SBC<IndirectY>();
    case 0xF5: return SBC<ZeroPageX>();
    case 0xF6: return INC<ZeroPageX>();
    case 0xF8: return SED();
    case 0xF9: return SBC<AbsoluteY>();
    case 0xFD: return SBC<AbsoluteX>();
    case 0xFE: return INC<AbsoluteX_Exception>();

    //
    // Unofficial instructions
    //

    // NOP
    case 0x04:
    case 0x44:
    case 0x64: return NOP<ZeroPage>();
    case 0x0C: return NOP<Absolute>();
    case 0x14:
    case 0x34:
    case 0x54:
    case 0x74:
    case 0xD4:
    case 0xF4: return NOP<ZeroPageX>();
    case 0x1A:
    case 0x3A:
    case 0x5A:
    case 0x7A:
    case 0xDA:
    case 0xFA: return NOP();
    case 0x80: return NOP<Immediate>();
    case 0x1C:
    case 0x3C:
    case 0x5C:
    case 0x7C:
    case 0xDC:
    case 0xFC: return NOP<AbsoluteX>();

    // LAX
    case 0xA3: return LAX<IndirectX>();
    case 0xA7: return LAX<ZeroPage>();
    case 0xAF: return LAX<Absolute>();
    case 0xB3: return LAX<IndirectY>();
    case 0xB7: return LAX<ZeroPageY>();
    case 0xBF: return LAX<AbsoluteY>();

    // SAX
    case 0x83: return SAX<IndirectX>();
    case 0x87: return SAX<ZeroPage>();
    case 0x8F: return SAX<Absolute>();
    case 0x97: return SAX<ZeroPageY>();

    // SBC
    case 0xEB: return SBC<Immediate>();

    // DCP
    case 0xC3: return DCP<IndirectX>();
    case 0xC7: return DCP<ZeroPage>();
    case 0xCF: return DCP<Absolute>();
    case 0xD3: return DCP<IndirectY>();
    case 0xD7: return DCP<ZeroPageX>();
    case 0xDB: return DCP<AbsoluteY>();
    case 0xDF: return DCP<AbsoluteX>();

    // ISB
    case 0xE3: return ISB<IndirectX>();
    case 0xE7: return ISB<ZeroPage>();
    case 0xEF: return ISB<Absolute>();
    case 0xF3: return ISB<IndirectY>();
    case 0xF7: return ISB<ZeroPageX>();
    case 0xFB: return ISB<AbsoluteY>();
    case 0xFF: return ISB<AbsoluteX>();

    // SLO
    case 0x03: return SLO<IndirectX>();
    case 0x07: return SLO<ZeroPage>();
    case 0x0F: return SLO<Absolute>();
    case 0x13: return SLO<IndirectY>();
    case 0x17: return SLO<ZeroPageX>();
    case 0x1B: return SLO<AbsoluteY>();
    case 0x1F: return SLO<AbsoluteX>();

    // RLA
    case 0x23: return RLA<IndirectX>();
    case 0x27: return RLA<ZeroPage>();
    case 0x2F: return RLA<Absolute>();
    case 0x33: return RLA<IndirectY>();
    case 0x37: return RLA<ZeroPageX>();
    case 0x3B: return RLA<AbsoluteY>();
    case 0x3F: return RLA<AbsoluteX>();

    // SRE
    case 0x43: return SRE<IndirectX>();
    case 0x47: return SRE<ZeroPage>();
    case 0x4F: return SRE<Absolute>();
    case 0x53: return SRE<IndirectY>();
    case 0x57: return SRE<ZeroPageX>();
    case 0x5B: return SRE<AbsoluteY>();
    case 0x5F: return SRE<AbsoluteX>();

    // RRA
    case 0x63: return RRA<IndirectX>();
    case 0x67: return RRA<ZeroPage>();
    case 0x6F: return RRA<Absolute>();
    case 0x73: return RRA<IndirectY>();
    case 0x77: return RRA<ZeroPageX>();
    case 0x7B: return RRA<AbsoluteY>();
    case 0x7F: return RRA<AbsoluteX>();

    default: {
      LOG(log::Error) << "Invalid opcode: " << +opcode;
      throw std::runtime_error("Invalid opcode");
      return;
    }
  }
}

/* Instructions */

//
// Auxiliary functions
//

void cpu::add(const uint8_t value)
{
  const uint16_t result = state.a + value + state.check_flags(flags::Carry);

  state.clear_flags(flags::Carry | flags::Overflow);

  if (result > 0xFF) {
    state.set_flags(flags::Carry);
  }

  if (~(state.a ^ value) & (state.a ^ result) & 0x80) {
    state.set_flags(flags::Overflow);
  }

  state.set_a(static_cast<uint8_t>(result));
}

uint8_t cpu::shift_left(const uint8_t value)
{
  const uint8_t result = value << 1;
  state.update_nz(result);

  state.clear_flags(flags::Carry);

  if (value & 0x80) {
    state.set_flags(flags::Carry);
  }

  return result;
}

uint8_t cpu::shift_right(const uint8_t value)
{
  const uint8_t result = value >> 1;
  state.update_nz(result);

  state.clear_flags(flags::Carry);

  if (value & 0x01) {
    state.set_flags(flags::Carry);
  }

  return result;
}

uint8_t cpu::rotate_left(const uint8_t value)
{
  const uint8_t result = (value << 1) | state.check_flags(flags::Carry);
  state.update_nz(result);

  state.clear_flags(flags::Carry);

  if (value & 0x80) {
    state.set_flags(flags::Carry);
  }

  return result;
}

uint8_t cpu::rotate_right(const uint8_t value)
{
  const uint8_t result = (value >> 1) | (state.check_flags(flags::Carry) << 7);
  state.update_nz(result);

  state.clear_flags(flags::Carry);

  if (value & 0x01) {
    state.set_flags(flags::Carry);
  }

  return result;
}

void cpu::compare(const uint8_t reg, const uint8_t value)
{
  state.update_nz(reg - value);

  state.clear_flags(flags::Carry);

  if (reg >= value) {
    state.set_flags(flags::Carry);
  }
}

void cpu::branch(const bool taken)
{
  const auto addr   = get_operand<Relative>();
  const auto offset = static_cast<int8_t>(memory_read(addr));

  if (taken) {
    if (crosses_page(state.pc, offset)) {
      tick();
    }

    tick();

    this->state.pc += offset;
  }
}

void cpu::push(const uint8_t value)
{
  memory_write(0x100 + state.sp, value);
  --state.sp;
}

uint8_t cpu::pop()
{
  ++state.sp;
  return memory_read(0x100 + state.sp);
}

bool cpu::crosses_page(const uint16_t addr, const uint8_t i) const
{
  return ((addr + i) & 0xFF00) != ((addr & 0xFF00));
}

bool cpu::crosses_page(const uint16_t addr, const int8_t i) const
{
  return ((addr + i) & 0xFF00) != ((addr & 0xFF00));
}

//
// Storage
//

template <auto Mode> void cpu::LDA()
{
  const auto addr  = get_operand<Mode>();
  const auto value = memory_read(addr);

  state.set_a(value);
}

template <auto Mode> void cpu::LDX()
{
  const auto addr  = get_operand<Mode>();
  const auto value = memory_read(addr);

  state.set_x(value);
}

template <auto Mode> void cpu::LDY()
{
  const auto addr  = get_operand<Mode>();
  const auto value = memory_read(addr);

  state.set_y(value);
}

template <auto Mode> void cpu::STA()
{
  const auto addr = get_operand<Mode>();

  if constexpr (
      Mode == AbsoluteX_Exception || Mode == AbsoluteY_Exception ||
      Mode == IndirectY_Exception) {
    tick();
  }

  memory_write(addr, state.a);
}

template <auto Mode> void cpu::STX()
{
  const auto addr = get_operand<Mode>();

  memory_write(addr, state.x);
}

template <auto Mode> void cpu::STY()
{
  const auto addr = get_operand<Mode>();

  memory_write(addr, state.y);
}

void cpu::TAX()
{
  state.set_x(state.a);
  tick();
}

void cpu::TAY()
{
  state.set_y(state.a);
  tick();
}

void cpu::TSX()
{
  state.set_x(state.sp);
  tick();
}

void cpu::TXA()
{
  state.set_a(state.x);
  tick();
}

void cpu::TXS()
{
  state.sp = state.x;
  tick();
}

void cpu::TYA()
{
  state.set_a(state.y);
  tick();
}

//
// Math
//

template <auto Mode> void cpu::ADC()
{
  const auto addr  = get_operand<Mode>();
  const auto value = memory_read(addr);

  this->add(value);
}

template <auto Mode> void cpu::SBC()
{
  const auto addr  = get_operand<Mode>();
  const auto value = memory_read(addr);

  this->add(value ^ 0xFF);
}

template <auto Mode> void cpu::INC()
{
  const auto addr  = get_operand<Mode>();
  const auto value = memory_read(addr);

  if constexpr (Mode == AbsoluteX_Exception) {
    tick();
  }

  const uint8_t result = value + 1;
  state.update_nz(result);

  tick();
  memory_write(addr, result);
}

template <auto Mode> void cpu::DEC()
{
  const auto addr  = get_operand<Mode>();
  const auto value = memory_read(addr);

  if constexpr (Mode == AbsoluteX_Exception) {
    tick();
  }

  const uint8_t result = value - 1;
  state.update_nz(result);

  tick();
  memory_write(addr, result);
}

void cpu::INX()
{
  tick();
  state.set_x(state.x + 1);
}

void cpu::INY()
{
  tick();
  state.set_y(state.y + 1);
}

void cpu::DEX()
{
  tick();
  state.set_x(state.x - 1);
}

void cpu::DEY()
{
  tick();
  state.set_y(state.y - 1);
}

//
// Bitwise
//

template <auto Mode> void cpu::AND()
{
  const auto addr  = get_operand<Mode>();
  const auto value = memory_read(addr);

  state.set_a(state.a & value);
}

template <auto Mode> void cpu::ORA()
{
  const auto addr  = get_operand<Mode>();
  const auto value = memory_read(addr);

  state.set_a(state.a | value);
}

template <auto Mode> void cpu::EOR()
{
  const auto addr  = get_operand<Mode>();
  const auto value = memory_read(addr);

  state.set_a(state.a ^ value);
}

template <auto Mode> void cpu::LSR()
{
  if constexpr (Mode == Accumulator) {
    tick();
    state.set_a(shift_right(state.a));
  } else {
    const auto addr  = get_operand<Mode>();
    const auto value = memory_read(addr);

    if constexpr (Mode == AbsoluteX_Exception) {
      tick();
    }

    tick();
    memory_write(addr, shift_right(value));
  }
}

template <auto Mode> void cpu::ASL()
{
  if constexpr (Mode == Accumulator) {
    tick();
    state.set_a(shift_left(state.a));
  } else {
    const auto addr  = get_operand<Mode>();
    const auto value = memory_read(addr);

    if constexpr (Mode == AbsoluteX_Exception) {
      tick();
    }

    tick();
    memory_write(addr, shift_left(value));
  }
}

template <auto Mode> void cpu::ROL()
{
  if constexpr (Mode == Accumulator) {
    tick();
    state.set_a(rotate_left(state.a));
  } else {
    const auto addr  = get_operand<Mode>();
    const auto value = memory_read(addr);

    if constexpr (Mode == AbsoluteX_Exception) {
      tick();
    }

    tick();
    memory_write(addr, rotate_left(value));
  }
}

template <auto Mode> void cpu::ROR()
{
  if constexpr (Mode == Accumulator) {
    tick();
    state.set_a(rotate_right(state.a));
  } else {
    const auto addr  = get_operand<Mode>();
    const auto value = memory_read(addr);

    if constexpr (Mode == AbsoluteX_Exception) {
      tick();
    }

    tick();
    memory_write(addr, rotate_right(value));
  }
}

//
// Flags
//

void cpu::CLC()
{
  tick();
  state.clear_flags(flags::Carry);
}

void cpu::CLD()
{
  tick();
  state.clear_flags(flags::Decimal);
}

void cpu::CLI()
{
  tick();
  state.clear_flags(flags::Interrupt);
}

void cpu::CLV()
{
  tick();
  state.clear_flags(flags::Overflow);
}

void cpu::SEC()
{
  tick();
  state.set_flags(flags::Carry);
}

void cpu::SED()
{
  tick();
  state.set_flags(flags::Decimal);
}

void cpu::SEI()
{
  tick();
  state.set_flags(flags::Interrupt);
}

template <auto Mode> void cpu::CMP()
{
  const auto addr  = get_operand<Mode>();
  const auto value = memory_read(addr);

  compare(state.a, value);
}

template <auto Mode> void cpu::CPX()
{
  const auto addr  = get_operand<Mode>();
  const auto value = memory_read(addr);

  compare(state.x, value);
}

template <auto Mode> void cpu::CPY()
{
  const auto addr  = get_operand<Mode>();
  const auto value = memory_read(addr);

  compare(state.y, value);
}

template <auto Mode> void cpu::BIT()
{
  const auto addr  = get_operand<Mode>();
  const auto value = memory_read(addr);

  state.clear_flags(flags::Zero | flags::Overflow | flags::Negative);

  if (!(state.a & value)) {
    state.set_flags(flags::Zero);
  }

  if (value & 0x40) {
    state.set_flags(flags::Overflow);
  }

  if (value & 0x80) {
    state.set_flags(flags::Negative);
  }
}

//
// Jumps and branches
//

template <auto Mode> void cpu::JMP()
{
  state.set_pc(get_operand<Mode>());
}

void cpu::JSR()
{
  tick();
  push(static_cast<uint8_t>((state.pc + 1) >> 8));
  push(static_cast<uint8_t>(state.pc + 1));

  state.set_pc(get_operand<Absolute>());
}

void cpu::BCC()
{
  branch(state.check_flags(flags::Carry) == false);
}

void cpu::BCS()
{
  branch(state.check_flags(flags::Carry) == true);
}

void cpu::BEQ()
{
  branch(state.check_flags(flags::Zero) == true);
}

void cpu::BMI()
{
  branch(state.check_flags(flags::Negative) == true);
}

void cpu::BNE()
{
  branch(state.check_flags(flags::Zero) == false);
}

void cpu::BPL()
{
  branch(state.check_flags(flags::Negative) == false);
}

void cpu::BVC()
{
  branch(state.check_flags(flags::Overflow) == false);
}

void cpu::BVS()
{
  branch(state.check_flags(flags::Overflow) == true);
}

void cpu::RTS()
{
  tick();
  tick();
  tick();
  state.set_pc((pop() | (pop() << 8)) + 1);
}

void cpu::RTI()
{
  tick();
  tick();
  state.set_ps(pop());
  state.set_pc(pop() | (pop() << 8));
}

//
// Stack
//

void cpu::PHA()
{
  tick();
  push(state.a);
}

void cpu::PLA()
{
  tick();
  tick();
  state.set_a(pop());
}

void cpu::PHP()
{
  tick();
  push(state.ps | flags::Break | flags::Reserved);
}

void cpu::PLP()
{
  tick();
  tick();
  state.set_ps(pop());
}

//
// System
//

void cpu::INT_NMI()
{
  tick();
  tick();

  push(state.pc >> 8);
  push(state.pc & 0xFF);
  push(state.ps);

  state.set_flags(flags::Interrupt);

  constexpr uint16_t jmp_addr = 0xFFFA;

  state.pc       = (memory_read(jmp_addr + 1) << 8) | memory_read(jmp_addr);
  state.nmi_flag = false;
}

void cpu::INT_RST()
{
  tick();
  tick();

  state.sp -= 3;
  tick();
  tick();
  tick();

  state.set_flags(flags::Interrupt);

  constexpr uint16_t jmp_addr = 0xFFFC;
  state.pc = (memory_read(jmp_addr + 1) << 8) | memory_read(jmp_addr);
}

void cpu::INT_IRQ()
{
  tick();
  tick();

  push(state.pc >> 8);
  push(state.pc & 0xFF);
  push(state.ps);

  state.set_flags(flags::Interrupt);

  constexpr uint16_t jmp_addr = 0xFFFE;
  state.pc = (memory_read(jmp_addr + 1) << 8) | memory_read(jmp_addr);
}

void cpu::INT_BRK()
{
  tick();

  push(state.pc >> 8);
  push(state.pc & 0xFF);
  push(state.ps | flags::Break | flags::Reserved);

  state.set_flags(flags::Interrupt);

  constexpr uint16_t jmp_addr = 0xFFFE;
  state.pc = (memory_read(jmp_addr + 1) << 8) | memory_read(jmp_addr);
}

void cpu::NOP()
{
  tick();
}

//
// Unofficial instructions
//

template <auto Mode> void cpu::NOP()
{
  get_operand<Mode>();  // Discard it
  tick();
}

template <auto Mode> void cpu::LAX()
{
  const auto addr  = get_operand<Mode>();
  const auto value = memory_read(addr);

  state.set_x(value);
  state.set_a(value);
}

template <auto Mode> void cpu::SAX()
{
  const auto addr = get_operand<Mode>();

  memory_write(addr, state.a & state.x);
}

template <auto Mode> void cpu::DCP()
{
  const auto    addr  = get_operand<Mode>();
  const uint8_t value = memory_read(addr) - 1;

  tick();

  state.clear_flags(flags::Carry);

  if (state.a >= value) {
    state.set_flags(flags::Carry);
  }

  state.update_nz(state.a - value);

  memory_write(addr, value);
}

template <auto Mode> void cpu::ISB()
{
  const auto    addr  = get_operand<Mode>();
  const uint8_t value = memory_read(addr) + 1;

  tick();

  add(value ^ 0xFF);
  memory_write(addr, value);
}

template <auto Mode> void cpu::SLO()
{
  const auto addr  = get_operand<Mode>();
  const auto value = memory_read(addr);

  tick();

  const auto shifted = shift_left(value);

  state.set_a(state.a | shifted);
  memory_write(addr, shifted);
}

template <auto Mode> void cpu::RLA()
{
  const auto addr  = get_operand<Mode>();
  const auto value = memory_read(addr);

  tick();

  const auto shifted = rotate_left(value);

  state.set_a(state.a & shifted);
  memory_write(addr, shifted);
}

template <auto Mode> void cpu::SRE()
{
  const auto addr  = get_operand<Mode>();
  const auto value = memory_read(addr);

  tick();

  const auto shifted = shift_right(value);

  state.set_a(state.a ^ shifted);
  memory_write(addr, shifted);
}

template <auto Mode> void cpu::RRA()
{
  const auto addr  = get_operand<Mode>();
  const auto value = memory_read(addr);

  tick();

  const auto result = rotate_right(value);

  add(result);
  memory_write(addr, result);
}

template <auto Mode> uint16_t cpu::get_operand()
{
  if constexpr (Mode != Immediate && Mode != Relative) {
    throw;
  }
  const auto addr = state.pc;

  ++state.pc;

  return addr;
}

template <> uint16_t cpu::get_operand<ZeroPage>()
{
  return memory_read(get_operand<Immediate>());
}

template <> uint16_t cpu::get_operand<ZeroPageX>()
{
  tick();

  return (get_operand<ZeroPage>() + state.x) & 0xFF;
}

template <> uint16_t cpu::get_operand<ZeroPageY>()
{
  tick();

  return (get_operand<ZeroPage>() + state.y) & 0xFF;
}

template <> uint16_t cpu::get_operand<Absolute>()
{
  const auto base_addr = get_operand<Immediate>();

  ++state.pc;

  return (memory_read(base_addr + 1) << 8) | memory_read(base_addr);
}

template <> uint16_t cpu::get_operand<AbsoluteX>()
{
  const auto base_addr = get_operand<Absolute>();

  if (crosses_page(base_addr, state.x)) {
    tick();
  }

  return base_addr + state.x;
}

template <> uint16_t cpu::get_operand<AbsoluteX_Exception>()
{
  const auto base_addr = get_operand<Absolute>();

  return base_addr + state.x;
}

template <> uint16_t cpu::get_operand<AbsoluteY>()
{
  const auto base_addr = get_operand<Absolute>();

  if (crosses_page(base_addr, state.y)) {
    tick();
  }

  return base_addr + state.y;
}

template <> uint16_t cpu::get_operand<AbsoluteY_Exception>()
{
  const auto base_addr = get_operand<Absolute>();

  return base_addr + state.y;
}

template <> uint16_t cpu::get_operand<Indirect>()
{
  const auto base_addr = get_operand<Absolute>();
  return memory_read(base_addr) |
         (memory_read((base_addr & 0xFF00) | ((base_addr + 1) & 0xFF)) << 8);
}

template <> uint16_t cpu::get_operand<IndirectX>()
{
  const auto base_addr = get_operand<ZeroPageX>();
  return (memory_read((base_addr + 1) & 0xFF) << 8) | memory_read(base_addr);
}

template <> uint16_t cpu::get_operand<IndirectY>()
{
  const auto     zp_addr = get_operand<ZeroPage>();
  const uint16_t base_addr =
      ((memory_read((zp_addr + 1) & 0xFF) << 8) | memory_read(zp_addr));

  if (crosses_page(base_addr, state.y)) {
    tick();
  }

  return base_addr + state.y;
}

template <> uint16_t cpu::get_operand<IndirectY_Exception>()
{
  const auto     zp_addr = get_operand<ZeroPage>();
  const uint16_t base_addr =
      ((memory_read((zp_addr + 1) & 0xFF) << 8) | memory_read(zp_addr));

  return base_addr + state.y;
}
}  // namespace nes