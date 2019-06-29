#pragma once

#include <cstdint>

namespace platform
{
	class input
	{
		uint8_t const * keys;

		input(input const &) = delete;
		input(input &&)		 = delete;
		input & operator=(input const &) = delete;
		input & operator=(input &&) = delete;

	  public:
		input();

		uint8_t get_controller(int n) const;
	};
}
