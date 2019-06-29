#include "memory_mapper.h"

#include "apu.h"
#include "cartridge.h"
#include "controller.h"
#include "ppu.h"
#include "ram.h"

namespace nes
{
	memory_mapper::memory_mapper(nes::ram & ram, nes::ppu & ppu, nes::apu & apu, nes::controller & controller, nes::cartridge & cartridge) :
		ram{ ram },
		ppu{ ppu },
		apu{ apu },
		controller{ controller },
		cartridge{ cartridge }
	{}

	uint8_t memory_mapper::read(uint16_t addr) const
	{
		if (addr & 0x8000) goto prg;
		if (~addr & 0x4000) goto low;
		if (addr & 0x2000) goto prg;
		if (addr & 0x1FE0) goto err;
		if (~addr & 0x0010) goto apu;
		if (addr & 0x0008) goto err;
		if (~addr & 0x0004) goto apu;
		if (addr & 0x0002) goto ctl;
		if (addr & 0x0001) goto apu;
	err:
		throw std::runtime_error(std::string{ "Illegal read at " } + std::to_string(addr));
	low:
		return (addr & 0x2000) ? ppu.read(addr % 0x8) : ram.read(addr % 0x800);
	apu:
		return apu.read(0);
	ctl:
		return controller.read(addr & 0x0001);
	prg:
		return cartridge.prg_read(addr);
	}
	void memory_mapper::write(uint16_t addr, uint8_t value)
	{
		if (addr & 0x8000) goto prg;
		if (~addr & 0x4000) goto low;
		if (addr & 0x3FE0) goto prg;
		if (~addr & 0x0010) goto apu;
		if (addr & 0x0008) goto err;
		if (~addr & 0x0004) goto apu;
		if (addr & 0x0001) goto apu;
		if (addr & 0x0002) goto ctl;
	err:
		throw std::runtime_error(std::string{ "Illegal write at " } + std::to_string(addr));
	low:
		return (addr & 0x2000) ? ppu.write(addr % 0x8, value) : ram.write(addr % 0x800, value);
	apu:
		return apu.write(addr, value);
	ctl:
		return controller.write(value & 0x0001);
	prg:
		return cartridge.prg_write(addr, value);
	}
}
