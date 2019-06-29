#pragma once

#include <array>

namespace platform
{
	class input;
}

namespace nes
{
	class controller
	{
		platform::input & input;

		bool				   strobe = false;		 // strobe latch
		std::array<uint8_t, 2> controller_bits{};	// shift registers

		controller(controller const &) = delete;
		controller(controller &&)	  = delete;
		controller & operator=(controller const &) = delete;
		controller & operator=(controller &&) = delete;

	public:
		controller(platform::input & input);

		uint8_t read(size_t);
		void	write(bool);
	};
}
