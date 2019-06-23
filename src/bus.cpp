#include "bus.h"

#include "apu.h"
#include "cartridge.h"
#include "controller.h"
#include "cpu.h"
#include "debugger.h"
#include "emulator.h"
#include "log.h"
#include "ppu.h"

namespace nes {
void bus::set_component(nes::cpu& ref)
{
  this->cpu = &ref;
}

void bus::set_component(nes::ppu& ref)
{
  this->ppu = &ref;
}

void bus::set_component(nes::apu& ref)
{
  this->apu = &ref;
}

void bus::set_component(nes::cartridge& ref)
{
  this->cartridge = &ref;
}

void bus::set_component(nes::controller& ref)
{
  this->controller = &ref;
}

void bus::set_component(nes::debugger& ref)
{
  this->debugger = &ref;
}

void bus::set_component(nes::emulator& ref)
{
  this->emulator = &ref;
}

//
// CPU access
//

void bus::run_frame()
{
  this->cpu->run_frame();
}

void bus::set_nmi(const bool value)
{
  this->cpu->set_nmi(value);
}

void bus::set_irq(const bool value)
{
  this->cpu->set_irq(value);
}

//
// PPU access
//

void bus::ppu_step()
{
  this->ppu->step();
}

uint8_t bus::ppu_read(const uint16_t addr)
{
  return this->ppu->read(addr);
}

void bus::ppu_write(const uint16_t addr, const uint8_t value)
{
  this->ppu->write(addr, value);
}

void bus::set_mirroring(const int mode)
{
  this->ppu->set_mirroring(mode);
}

//
// APU access
//

uint8_t bus::apu_read(const int elapsed)
{
  return this->apu->read(elapsed);
}

void bus::apu_write(const int elapsed, const uint16_t addr, const uint8_t value)
{
  this->apu->write(elapsed, addr, value);
}

//
// Cartridge access
//

uint8_t bus::prg_read(const uint16_t addr) const
{
  return this->cartridge->prg_read(addr);
}

void bus::prg_write(const uint16_t addr, const uint8_t value)
{
  this->cartridge->prg_write(addr, value);
}

uint8_t bus::chr_read(const uint16_t addr) const
{
  return this->cartridge->chr_read(addr);
}

void bus::chr_write(const uint16_t addr, uint8_t value)
{
  this->cartridge->chr_write(addr, value);
}

//
// Controller access
//

uint8_t bus::controller_read(size_t port) const
{
  return this->controller->read(port);
}

void bus::controller_write(bool value)
{
  this->controller->write(value);
}

//
// Debugger access
//

void bus::nestest()
{
  this->debugger->nestest();
}

//
// Emulator access
//

void bus::update_frame(const uint32_t* frame)
{
  this->emulator->update_frame(frame);
}

uint8_t bus::get_controller(const size_t port) const
{
  return this->emulator->get_controller(port);
}
}  // namespace nes