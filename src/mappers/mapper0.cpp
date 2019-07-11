#include "mappers/mapper0.h"

#include "ppu.h"

namespace nes
{
	mapper0::mapper0(
		nes::cartridge_info &   info,
		std::vector<uint8_t> && prg,
		std::vector<uint8_t> && chr) :
		mapper{ info, std::move(prg), std::move(chr) }
	{}

	void mapper0::reset()
	{
		set_prg_map<16>(0, 0);
		set_prg_map<16>(1, 1);
		set_chr_map<8>(0, 0);
	}
}
