#pragma once

#include <cstdint>

namespace platform
{
	class display;
}
namespace nes
{
	class cartridge;
	class nmi_flipflop;

	class ppu
	{
		ppu(ppu const &) = delete;
		ppu(ppu &&)		 = delete;
		ppu & operator=(ppu const &) = delete;
		ppu & operator=(ppu &&) = delete;

	public:
		ppu(nes::cartridge & cartridge, platform::display & display, nes::nmi_flipflop & nmi_flipflop);

		void	reset();
		uint8_t read(uint16_t);
		void	write(uint16_t, uint8_t);
		void	set_mirroring(int);
		void	clock();
	};
}
