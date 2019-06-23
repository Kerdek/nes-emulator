#include <fstream>

#include "apu.h"
#include "bus.h"
#include "cartridge.h"
#include "controller.h"
#include "cpu.h"
#include "debugger.h"
#include "emulator.h"
#include "log.h"
#include "ppu.h"

int main()
{
  std::ofstream log_file{"nes-emulator.log"};

  nes::log::get().set_stream(log_file);
  nes::log::get().set_level(nes::log::Info);

  nes::bus        bus;
  nes::cpu        cpu;
  nes::ppu        ppu;
  nes::apu        apu;
  nes::cartridge  cartridge;
  nes::controller controller;
  nes::emulator   emulator;
  // nes::debugger   debugger{cpu};

  bus.set_component(cpu);
  bus.set_component(ppu);
  bus.set_component(apu);
  bus.set_component(cartridge);
  bus.set_component(controller);
  bus.set_component(emulator);
  // bus.set_component(debugger);

  cpu.set_bus(bus);
  ppu.set_bus(bus);
  apu.set_bus(bus);
  cartridge.set_bus(bus);
  controller.set_bus(bus);
  emulator.set_bus(bus);

  cartridge.load("../roms/ff.nes");
  cpu.power_on();
  ppu.power_on();
  apu.power_on();

  emulator.run();

  return 0;
}