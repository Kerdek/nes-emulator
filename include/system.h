#pragma once

#include "apu.h"
#include "cartridge.h"
#include "controller.h"
#include "cpu.h"
#include "debugger.h"
#include "memory_mapper.h"
#include "ppu.h"
#include "ram.h"

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
	void line_cycle();
	void step();
}

namespace nes
{
	class system
	{
		friend uint8_t PPU::read_memory(uint16_t addr);
		friend void	PPU::write_memory(uint16_t addr, uint8_t v);
		friend void PPU::step();

		nes::ram		   ram;
		nes::ppu		   ppu;
		nes::apu		   apu;
		nes::controller	controller;
		nes::cartridge	 cartridge;
		nes::memory_mapper memory_mapper;
		nes::cpu		   cpu;
		nes::debugger	  debugger;

		system(system const &) = delete;
		system(system &&)	  = delete;
		system & operator=(system const &) = delete;
		system & operator=(system &&) = delete;

	public:
		system(std::filesystem::path const & rom, platform::input & input, platform::display & display);
		void run_frame();
	};
}
