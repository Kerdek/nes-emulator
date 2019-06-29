#pragma once

#include "cpu.h"

#include <fstream>

#ifdef _DEBUG
constexpr auto debug_mode = true;
#else
constexpr auto debug_mode = false;
#endif

namespace nes
{
	class debugger
	{
	  public:
		debugger(nes::cpu const & cpu_in) :
		  cpu(cpu_in)
		{}

		void nestest();

		debugger(debugger const &) = delete;
		debugger(debugger &&)	  = delete;
		debugger & operator=(debugger const &) = delete;
		debugger & operator=(debugger &&) = delete;

	  private:
		std::ofstream	nestest_log{ "../roms/nestest_out.log" };
		nes::cpu const & cpu;

		enum addr_mode2
		{
			impl,
			acc,
			imm,
			zp,
			zpx,
			zpy,
			rel,
			ab,
			abx,
			abx_,
			aby,
			aby_,
			ind,
			indx,
			indy,
			indy_,
			inv
		};
	};
}
