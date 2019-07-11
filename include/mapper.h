#pragma once

#include <array>
#include <cstdint>
#include <vector>

namespace nes
{
	class ppu;

	namespace mirroring
	{
		enum mirroring
		{
			Horizontal,
			Vertical
		};
	}

	struct cartridge_info
	{
		uint32_t rom_size;
		uint8_t  mapper_num;
		uint16_t prg_banks;
		uint16_t chr_banks;
		uint32_t prg_ram_size;
		int		 mirroring;
		bool	 chr_ram = false;
	};
	class mapper
	{
		mapper(mapper const &) = delete;
		mapper(mapper &&)	  = delete;
		mapper & operator=(mapper const &) = delete;
		mapper & operator=(mapper &&) = delete;

	protected:
		nes::cartridge_info &   info;
		std::vector<uint8_t>	prg;
		std::vector<uint8_t>	chr;
		std::vector<uint8_t>	prg_ram;
		std::array<uint32_t, 4> prg_map{};
		std::array<uint32_t, 8> chr_map{};

	public:
		mapper(
			nes::cartridge_info &   info,
			std::vector<uint8_t> && prg,
			std::vector<uint8_t> && chr);

		virtual ~mapper()	= default;
		virtual void reset() = 0;
		virtual void prg_write(uint16_t, uint8_t);
		virtual void chr_write(uint16_t, uint8_t);

		uint8_t prg_read(uint16_t) const;
		uint8_t chr_read(uint16_t) const;

		template<auto size>
		void set_prg_map(int, int);
		template<auto size>
		void set_chr_map(int, int);
	};
}
