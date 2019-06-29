#pragma once

#include "apu.h"
#include "cartridge.h"
#include "controller.h"
#include "cpu.h"
#include "debugger.h"
#include "ppu.h"

namespace PPU
{
	enum Scanline
	{
		VISIBLE,
		POST,
		NMI,
		PRE
	};

  uint8_t read_memory(uint16_t addr);
  void	write_memory(uint16_t addr, uint8_t v);
	template<Scanline s>
	void scanline_cycle();
}

namespace nes
{
	class system
	{
    friend uint8_t PPU::read_memory(uint16_t addr);
    friend void	PPU::write_memory(uint16_t addr, uint8_t v);
		template<PPU::Scanline s>
		friend void PPU::scanline_cycle();

		nes::ppu		ppu;
		nes::cartridge  cartridge;
		nes::controller controller;
		nes::apu		apu;
		nes::cpu		cpu;
		nes::debugger   debugger;

		system(system const &) = delete;
		system(system &&)	  = delete;
		system & operator=(system const &) = delete;
		system & operator=(system &&) = delete;

	  public:
		system(std::filesystem::path const & rom, platform::input & input, platform::display & display);
		void run_frame();
	};
}
