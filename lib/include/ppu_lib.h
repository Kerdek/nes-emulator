#pragma once

#include "cartridge.h"
#include "display.h"
#include "nmi_flipflop.h"

#include <array>

#define NTH_BIT(x, n) (((x) >> (n)) & 1)

enum ppu_map : uint8_t
{
	PPUCTRL   = 0x00,
	PPUMASK   = 0x01,
	PPUSTATUS = 0x02,
	OAMADDR   = 0x03,
	OAMDATA   = 0x04,
	PPUSCROLL = 0x05,
	PPUADDR   = 0x06,
	PPUDATA   = 0x07
};

constexpr uint32_t nes_palette[] =
{
	0x7C7C7C, 0xFC0000, 0xBC0000, 0xBC2844, 0x840094, 0x2000A8, 0x0010A8, 0x001488,	//
	0x003050, 0x007800, 0x006800, 0x005800, 0x584000, 0x000000, 0x000000, 0x000000,	//
	0xBCBCBC, 0xF87800, 0xF85800, 0xFC4468, 0xCC00D8, 0x5800E4, 0x0038F8, 0x105CE4,	//
	0x007CAC, 0x00B800, 0x00A800, 0x44A800, 0x888800, 0x000000, 0x000000, 0x000000,	//
	0xF8F8F8, 0xFCBC3C, 0xFC8868, 0xF87898, 0xF878F8, 0x9858F8, 0x5878F8, 0x44A0FC,	//
	0x00B8F8, 0x18F8B8, 0x54D858, 0x98F858, 0xD8E800, 0x787878, 0x000000, 0x000000,	//
	0xFCFCFC, 0xFCE4A4, 0xF8B8B8, 0xF8B8D8, 0xF8B8F8, 0xC0A4F8, 0xB0D0F0, 0xA8E0FC,	//
	0x78D8F8, 0x78F8D8, 0xB8F8B8, 0xD8F8B8, 0xFCFC00, 0xF8D8F8, 0x000000, 0x000000	//
};

namespace PPU
{
	struct sprite
	{
		uint8_t id;
		uint8_t x;
		uint8_t y;
		uint8_t tile;
		uint8_t attributes;
		uint8_t data_l;
		uint8_t data_h;
	};

	nes::cartridge *	cartridge;
	platform::display * display;
	nes::nmi_flipflop * nmi_flipflop;

	std::array<uint8_t , 0x0800> nametable_ram;
	std::array<uint8_t , 0x20>	 palette_ram;
	std::array<uint8_t , 0x0100> oam_memory;
	std::array<sprite  , 0x08>	 oam;
	std::array<sprite  , 0x08>	 secondary_oam;
	std::array<uint32_t, 0xF000> framebuffer;

	uint16_t
		v,
		t,
		line,
		dot,
		fetch_address,
		background_shift_register_l,
		background_shift_register_h;
	uint8_t
		x,
		w,
		oam_address,
		ctrl,
		mask,
		status,
		data_latch,
		data_buffer,
		mirroring_mode,
		name,
		attribute,
		background_tile_l,
		background_tile_h,
		attribute_shift_register_l,
		attribute_shift_register_h,
		attribute_latch_l,
		attribute_latch_h,
		odd_frame;
	void (*dot_process)();
	void dot_0_0();
	void dot_240_0();
	void dot_nt_odd();
	void dot_nt_even();

	inline bool bg_enabled()
	{
		return !!(0x18 & mask);
	}
	inline uint16_t nt_mirror(uint16_t address)
	{
		switch (mirroring_mode)
		{
		case nes::mirroring::Vertical:
			return address & 0x07FF;
		case nes::mirroring::Horizontal:
			return ((address >> 1) & 0x0400) + (address & 0x02FF);
		default:
			return address - 0x2000;
		}
	}
	inline uint16_t nt_addr()
	{
		return 0x2000 | (v & 0x0FFF);
	}
	inline uint16_t at_addr()
	{
		return (0x23C0)
			 | (0x0600 & (v     ))
			 | (0x0038 & (v >> 4))
			 | (0x0007 & (v >> 2));
	}
	inline uint16_t bg_address()
	{
		return (0x1000 & (ctrl <<  8))
			 | (0x0FF0 & (name <<  4))
			 | (0x0007 & (v    >> 12));
	}
	inline uint16_t map_palette_address(uint16_t address)
	{
		return (address & ~((0x0003 & address) ? 0x0000 : 0x0010)) & 0x001F;
	}
	inline uint8_t read_palette(uint16_t address)
	{
		return ((mask & 0x01) ? 0x18 : 0xFF) & palette_ram[map_palette_address(address)];
	}
	inline void write_palette(uint16_t address, uint8_t value)
	{
		palette_ram[map_palette_address(address)] = value;
	}
	inline uint8_t read_memory(uint16_t address)
	{
		if (address & 0xC000) throw std::runtime_error{ std::string{ "Illegal ppu read at " } + std::to_string(address) };
		if (~address & 0x3F00)
		{
			if (address & 0x2000) return nametable_ram[nt_mirror(address)];
			return cartridge->chr_read(address);
		}
		return read_palette(address);
	}
	inline void write_memory(uint16_t address, uint8_t value)
	{
		if (address & 0xC000) throw std::runtime_error{ std::string{ "Illegal ppu read at " } + std::to_string(address) };
		if (~address & 0x3F00)
		{
			if (address & 0x2000) nametable_ram[nt_mirror(address)] = value;
			else cartridge->chr_write(address, value);
		}
		else write_palette(address, value);
	}
	inline void h_scroll()
	{
		if (!bg_enabled()) return;
		if ((0x001F & v) == 0x001F) v ^= 0x041F;
		else v = (0xFFE0 & v) | (0x001F & (v + 1));
	}
	inline void v_scroll()
	{
		if (!bg_enabled()) return;
		uint16_t v1 = ~0x7000 & v;
		if ((0x7000 & v) == 0x7000)
		{
			v1 = ~0x03E0 & v1;
			if ((0x03E0 & v) == 0x03E0) v = v1;
			else if ((0x03E0 & v) == 0x03A0) v = v1 ^ 0x0800;
			else v = v1 | (0x03E0 & (v + 0x0020));
		}
		else v = v1 | (0x7000 & (v + 0x1000));
	}
	inline void h_update()
	{
		if (!bg_enabled()) return;
		v = (~0x041F & v)
		  | ( 0x041F & t);
	}
	inline void v_update()
	{
		if (!bg_enabled()) return;
		v = (~0x7BE0 & v)
		  | ( 0x7BE0 & t);
	}
	inline void reload_shift_registers()
	{
		background_shift_register_l = (background_shift_register_l & 0xFF00) | background_tile_l;
		background_shift_register_h = (background_shift_register_h & 0xFF00) | background_tile_h;
		attribute_latch_l = (attribute & 0x01);
		attribute_latch_h = (attribute & 0x02);
	}
	inline uint8_t sprite_height()
	{
		return (0x20 & ctrl) ? 0x10 : 0x08;
	}
	inline void clear_secondary_oam()
	{
		for (int i = 0; i < 8; i++)
		{
			secondary_oam[i].id	          = 64;
			secondary_oam[i].y		      = 0xFF;
			secondary_oam[i].tile         = 0xFF;
			secondary_oam[i].attributes   = 0xFF;
			secondary_oam[i].x		      = 0xFF;
			secondary_oam[i].data_l       = 0;
			secondary_oam[i].data_h       = 0;
		}
	}
	inline void evaluate_sprites()
	{
		if (line == 261) return;
		for (int i = 0, n = 0; i < 64; i++)
		{
			int sprite_line = line - oam_memory[i << 2];
			if (sprite_line >= 0 && sprite_line < sprite_height())
			{
				secondary_oam[n].id         = i;
				secondary_oam[n].y	        = oam_memory[(i << 2) + 0x0000];
				secondary_oam[n].tile       = oam_memory[(i << 2) + 0x0001];
				secondary_oam[n].attributes = oam_memory[(i << 2) + 0x0002];
				secondary_oam[n].x	        = oam_memory[(i << 2) + 0x0003];
				if(n++ == 8)
				{
					status |= 0x20;
					return;
				}
			}
		}
	}
	inline void load_sprites()
	{
		for (int i = 0; i < 8; i++)
		{
			oam[i] = secondary_oam[i];
			uint16_t address =
				(sprite_height() == 0x10)
					? (0x1000 & (oam[i].tile << 12)) | (0x0FE0 & (oam[i].tile << 4))
					: (0x1000 & (ctrl        <<  9)) | (         (oam[i].tile << 4));

			uint8_t sprite_line = (line - oam[i].y) % sprite_height();
			if (oam[i].attributes & 0x80) sprite_line ^= sprite_height() - 0x01;
			address += sprite_line + (sprite_line & 0x08);
			oam[i].data_l = read_memory(address + 0);
			oam[i].data_h = read_memory(address + 8);
		}
	}
	inline void pixel()
	{
		int16_t pxx = dot - 2;

		if (line < 240 && pxx >= 0 && pxx < 256)
		{
			uint8_t  palette     = 0;
			uint8_t  objPalette  = 0;
			uint8_t	 objPriority = 0;

			if ((0x08 & mask) && ((0x02 & mask) || pxx >= 8))
			{
				palette = (NTH_BIT(background_shift_register_h, 15 - x) << 1) | NTH_BIT(background_shift_register_l, 15 - x);
				if (palette) palette |= ((NTH_BIT(attribute_shift_register_h, 7 - x) << 1) | NTH_BIT(attribute_shift_register_l, 7 - x)) << 2;
			}
			if ((0x10 & mask) && ((0x04 & mask) || pxx >= 8))
			{
				for (int i = 7; i >= 0; i--)
				{
					if (oam[i].id == 64) continue;
					unsigned sprX = pxx - oam[i].x;
					if (sprX >= 8) continue;
					if (oam[i].attributes & 0x40) sprX ^= 7;

					uint8_t sprPalette = (NTH_BIT(oam[i].data_h, 7 - sprX) << 1) | NTH_BIT(oam[i].data_l, 7 - sprX);
					if (sprPalette == 0) continue;	// Transparent pixel.

					if (oam[i].id == 0 && palette && pxx != 255) status |= 0x40;

					sprPalette |= (oam[i].attributes & 3) << 2;
					objPalette  = sprPalette + 16;
					objPriority = oam[i].attributes & 0x20;
				}
			}
			if (objPalette && (palette == 0 || objPriority == 0)) palette = objPalette;
			framebuffer[line * 256 + pxx] = nes_palette[read_memory(0x3F00 + (bg_enabled() ? palette : 0))];
		}
		background_shift_register_l <<= 1;
		background_shift_register_h <<= 1;
		attribute_shift_register_l = (attribute_shift_register_l << 1) | attribute_latch_l;
		attribute_shift_register_h = (attribute_shift_register_h << 1) | attribute_latch_h;
	}

	void dot_340()
	{
		name = read_memory(fetch_address);
		if (line == 239) dot_process = &dot_240_0;
		else dot_process = &dot_0_0;
	}
	void dot_339()
	{
		fetch_address = nt_addr();
		dot_process = &dot_340;
	}
	void dot_338()
	{
		name = read_memory(fetch_address);
		dot_process = &dot_339;
	}
	void dot_321()
	{
		load_sprites();
		fetch_address = nt_addr();
		dot_process = &dot_nt_even;
	}
	void dot_0_258()
	{
		if (dot == 320) dot_process = &dot_321;
	}
	void dot_261_280()
	{
		v_update();
		if (dot == 304) dot_process = &dot_0_258;
	}
	void dot_261_258()
	{
		if (dot == 279) dot_process = &dot_261_280;
	}
	void dot_261_257()
	{
		pixel();
		reload_shift_registers();
		h_update();
		dot_process = &dot_261_258;
	}
	void dot_0_257()
	{
		evaluate_sprites();
		pixel();
		reload_shift_registers();
		h_update();
		dot_process = &dot_0_258;
	}
	void dot_256()
	{
		pixel();
		background_tile_h = read_memory(fetch_address);
		v_scroll();
		if (line == 261) dot_process = &dot_261_257;
		else dot_process = &dot_0_257;
	}
	void dot_bgh_even()
	{
		pixel();
		background_tile_h = read_memory(fetch_address);
		h_scroll();
		dot_process = &dot_nt_odd;
	}
	void dot_bgh_odd()
	{
		pixel();
		fetch_address += 8;
		if(dot == 255) dot_process = &dot_256;
		else dot_process = &dot_bgh_even;
	}
	void dot_bgl_even()
	{
		pixel();
		background_tile_l = read_memory(fetch_address);
		dot_process = &dot_bgh_odd;
	}
	void dot_bgl_odd()
	{
		pixel();
		fetch_address = bg_address();
		dot_process = &dot_bgl_even;
	}
	void dot_at_even()
	{
		pixel();
		attribute = read_memory(fetch_address);
		if (v & 0x02) attribute >>= 2;
		if (v & 0x40) attribute >>= 4;
		dot_process = &dot_bgl_odd;
	}
	void dot_at_odd()
	{
		pixel();
		fetch_address = at_addr();
		dot_process = &dot_at_even;
	}
	void dot_nt_even()
	{
		pixel();
		name = read_memory(fetch_address);
		dot_process = &dot_at_odd;
	}
	void dot_nt_odd()
	{
		pixel();
		fetch_address = nt_addr();
		reload_shift_registers();
		if(dot == 337) dot_process = &dot_338;
		else dot_process = &dot_nt_even;
	}
	void dot_261_1()
	{
		status &= ~0x60;
		fetch_address = nt_addr();
		status &= ~0x80;
		dot_process = &dot_nt_even;
	}
	void dot_241_2()
	{
		if (dot == 0 && line == 261) dot_process = &dot_261_1;
	}
	void dot_241_1()
	{
		status |= 0x80;
		if (ctrl & 0x80) nmi_flipflop->set();
		dot_process = &dot_241_2;
	}
	void dot_240_1()
	{
		if (dot == 0 && line == 241) dot_process = &dot_241_1;
	}
	void dot_240_0()
	{
		display->update_frame(framebuffer.data());
		dot_process = &dot_240_1;
	}
	void dot_0_1()
	{
		clear_secondary_oam();
		fetch_address = nt_addr();
		dot_process = &dot_nt_even;
	}
	void dot_0_0()
	{
		if (line == 0 && bg_enabled() && odd_frame)
		{
			++dot;
			dot_0_1();
		}
		else dot_process = &dot_0_1;
	}

	void clock()
	{
		dot_process();
		(dot = (dot + 1) % 341) || (line = (line + 1) % 262) || (odd_frame = !odd_frame);
	}
	void set_mirroring(int mode)
	{
		mirroring_mode = mode;
	}
	void write_register(uint8_t index, uint8_t value)
	{
		data_latch = value;

		switch (index)
		{
		case PPUCTRL:
			ctrl  = value;
			t = (~0x0C00 & t) | (0x0C00 & (ctrl << 10));
			break;
		case PPUMASK:
			mask = value;
			break;
		case OAMADDR:
			oam_address = value;
			break;
		case OAMDATA:
			oam_memory[oam_address++] = value;
			break;
		case PPUSCROLL:
			if ((w = !w))
			{
				x = value & 0x07;
				t = (~0x001F & t) | (0x001F & (value >> 3));
			}
			else
			{
				t = (~0x73E0 & t) | (0x7000 & (value << 12)) | (0x03E0 & (value << 2));
			}
			break;
		case PPUADDR:
			if ((w = !w))	t = (~0x7F00 & t) | (0x3F00 & (value << 8));
			else v = t = (~0x00FF & t) | (0x00FF & value);
			break;
		case PPUDATA:
			write_memory(0x3FFF & v, value);
			v = (~0x3FFF & v) | (0x3FFF & (v + ((0x04 & ctrl) ? 0x0020 : 0x0001)));
		}
	}
	uint8_t read_register(uint8_t index)
	{
		switch (index)
		{
		case PPUSTATUS:
			data_latch = (data_latch & 0x1F) | status;
			status &= 0x7F;
			w = 0;
			break;
		case OAMDATA:
			data_latch = oam_memory[oam_address];
			break;
		case PPUDATA:
			if (~v & 0x3F00)
			{
				data_latch  = data_buffer;
				data_buffer = read_memory(0x3FFF & v);
			}
			else
			{
				data_latch = read_memory(0x3FFF & v);
				data_buffer = read_memory(0x2FFF & v);
			}
			v = (~0x3FFF & v) | (0x3FFF & (v + ((0x04 & ctrl) ? 0x0020 : 0x0001)));
		}
		return data_latch;
	}
	void reset()
	{
		odd_frame    = false;
		line         = 0;
		dot_process  = dot_0_0;
		dot	         = 0;
		ctrl         = 0;
		mask         = 0;
		status       = 0;

		framebuffer.fill(0);
		nametable_ram.fill(0xFF);
		oam_memory.fill(0);
	}
}
