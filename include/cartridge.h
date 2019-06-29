#pragma once

#include "mapper.h"

#include <filesystem>
#include <memory>

namespace nes
{
	namespace ct
	{
		constexpr uint16_t prg_bank_size = 0x4000;	// 16384;
		constexpr uint16_t chr_bank_size = 0x2000;	// 8192;
	}

	class cartridge
	{
		nes::ppu & ppu;

		nes::cartridge_info			 info{};
		std::unique_ptr<nes::mapper> mapper = nullptr;

		cartridge(cartridge const &) = delete;
		cartridge(cartridge &&)		 = delete;
		cartridge & operator=(cartridge const &) = delete;
		cartridge & operator=(cartridge &&) = delete;

	public:
		cartridge(nes::ppu & ppu);

		void load(std::filesystem::path const &);

		uint8_t prg_read(uint16_t) const;
		uint8_t chr_read(uint16_t) const;

		void prg_write(uint16_t, uint8_t);
		void chr_write(uint16_t, uint8_t);
	};
}
