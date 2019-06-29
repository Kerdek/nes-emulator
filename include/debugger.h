#pragma once

#include <fstream>

#ifdef _DEBUG
constexpr auto debug_mode = true;
#else
constexpr auto debug_mode = false;
#endif

namespace nes
{
	class memory_mapper;
	class cpu;

	class debugger
	{
		std::ofstream			   nestest_log{ "../roms/nestest_out.log" };
		nes::memory_mapper const & memory_mapper;
		nes::cpu const &		   cpu;

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

		debugger(debugger const &) = delete;
		debugger(debugger &&)	  = delete;
		debugger & operator=(debugger const &) = delete;
		debugger & operator=(debugger &&) = delete;

		uint16_t peek_imm() const;
		uint16_t peek_zp() const;
		uint16_t peek_zpx() const;
		uint16_t peek_zpy() const;
		uint16_t peek_ab() const;
		uint16_t peek_abx() const;
		uint16_t peek_aby() const;
		uint16_t peek_ind() const;
		uint16_t peek_indx() const;
		uint16_t peek_indy() const;

	public:
		debugger(nes::memory_mapper const & memory_mapper, nes::cpu const & cpu) :
			memory_mapper{ memory_mapper },
			cpu{ cpu }
		{}

		void nestest();
	};
}
