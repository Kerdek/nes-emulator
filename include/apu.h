#pragma once

#include <cstdint>

namespace nes
{
	class apu
	{
		apu(apu const &) = delete;
		apu(apu &&)		 = delete;
		apu & operator=(apu const &) = delete;
		apu & operator=(apu &&) = delete;

	public:
		apu();
		void reset();

		uint8_t read(uint16_t address);
		void	write(uint16_t address, uint8_t value);

		void run_frame(int);
	};
}
