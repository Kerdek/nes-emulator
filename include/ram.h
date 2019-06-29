#pragma once

#include <array>
#include <cstdint>

namespace nes
{
	class ram
	{
		std::array<uint8_t, 0x800> data = {};

	public:
		void	reset();
		uint8_t read(uint16_t address);
		void	write(uint16_t address, uint8_t value);
	};
}
