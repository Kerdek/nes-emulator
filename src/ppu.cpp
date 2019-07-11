#include "ppu.h"

#include "ppu_lib.h"

namespace nes
{
	ppu::ppu(nes::cartridge & cartridge, platform::display & display, nes::nmi_flipflop & nmi_flipflop)
	{
		PPU::cartridge	= &cartridge;
		PPU::display	  = &display;
		PPU::nmi_flipflop = &nmi_flipflop;
	}
	void ppu::reset()
	{
		PPU::reset();
	}
	uint8_t ppu::read(uint16_t addr)
	{
		return PPU::read(addr % 8);
	}
	void ppu::write(uint16_t addr, uint8_t value)
	{
		PPU::write(addr % 8, value);
	}
	void ppu::clock()
	{
		PPU::clock();
	}
}
