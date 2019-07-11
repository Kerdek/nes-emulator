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

constexpr uint32_t nes_palette[] = {
	0x7C7C7C,
	0xFC0000,
	0xBC0000,
	0xBC2844,
	0x840094,
	0x2000A8,
	0x0010A8,
	0x001488,	//
	0x003050,
	0x007800,
	0x006800,
	0x005800,
	0x584000,
	0x000000,
	0x000000,
	0x000000,	//
	0xBCBCBC,
	0xF87800,
	0xF85800,
	0xFC4468,
	0xCC00D8,
	0x5800E4,
	0x0038F8,
	0x105CE4,	//
	0x007CAC,
	0x00B800,
	0x00A800,
	0x44A800,
	0x888800,
	0x000000,
	0x000000,
	0x000000,	//
	0xF8F8F8,
	0xFCBC3C,
	0xFC8868,
	0xF87898,
	0xF878F8,
	0x9858F8,
	0x5878F8,
	0x44A0FC,	//
	0x00B8F8,
	0x18F8B8,
	0x54D858,
	0x98F858,
	0xD8E800,
	0x787878,
	0x000000,
	0x000000,	//
	0xFCFCFC,
	0xFCE4A4,
	0xF8B8B8,
	0xF8B8D8,
	0xF8B8F8,
	0xC0A4F8,
	0xB0D0F0,
	0xA8E0FC,	//
	0x78D8F8,
	0x78F8D8,
	0xB8F8B8,
	0xD8F8B8,
	0xFCFC00,
	0xF8D8F8,
	0x000000,
	0x000000	//
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

	std::array<uint8_t, 0x0800>  nametable_ram;
	std::array<uint8_t, 0x20>	palette_ram;
	std::array<uint8_t, 0x0100>  oam_ram;
	std::array<sprite, 0x08>	 oam_0;
	std::array<sprite, 0x08>	 oam_1;
	std::array<uint32_t, 0xF000> framebuffer;

	uint16_t
		v,
		t,
		line,
		dot,
		address_buffer,
		background_shift_register_l,
		background_shift_register_h;

	uint8_t
		x,
		w,
		data_latch,
		data_buffer,
		oam_address,
		control_flags,
		mask_flags,
		status_flags,
		name_buffer,
		attribute_buffer,
		background_tile_l,
		background_tile_h,
		attribute_shift_register_l,
		attribute_shift_register_h,
		odd_frame,
		attribute_latch_l,
		attribute_latch_h;

	void (*dot_process)();
	void dot_0_0();
	void dot_240_0();
	void dot_nt_odd();
	void dot_nt_even();

	inline bool background_enabled()
	{
		return !!(0x18 & mask_flags);
	}
	inline uint16_t nametable_address()
	{
		return 0x2000 | (v & 0x0FFF);
	}
	inline uint16_t attributes_address()
	{
		return (0x23C0)
			   | (0x0C00 & (v))
			   | (0x0038 & (v >> 4))
			   | (0x0007 & (v >> 2));
	}
	inline uint16_t background_address()
	{
		return (0x1000 & (control_flags << 8))
			   | (0x0FF0 & (name_buffer << 4))
			   | (0x0007 & (v >> 12));
	}
	inline uint16_t map_nametable_address(uint16_t address)
	{
		switch (cartridge->get_mirroring())
		{
		case nes::mirroring::Vertical:
			return 0x07FF & address;
		case nes::mirroring::Horizontal:
			return (0x0400 & (address >> 1)) + (0x03FF & address);
		default:
			return address - 0x2000;
		}
	}
	inline uint16_t map_palette_address(uint16_t address)
	{
		return (address & ((0x0003 & address) ? ~0x0000 : ~0x0010)) & 0x001F;
	}
	inline uint8_t read_nametable(uint16_t address)
	{
		return nametable_ram[address];
	}
	inline void write_nametable(uint16_t address, uint8_t value)
	{
		nametable_ram[address] = value;
	}
	inline uint8_t read_palette(uint16_t address)
	{
		return ((mask_flags & 0x01) ? 0x18 : 0xFF) & palette_ram[address];
	}
	inline void write_palette(uint16_t address, uint8_t value)
	{
		palette_ram[address] = value;
	}
	inline uint8_t read_memory(uint16_t address)
	{
		if (address & 0xC000) throw std::runtime_error{ std::string{ "Illegal ppu read at " } + std::to_string(address) };
		if (~address & 0x3F00)
		{
			if (address & 0x2000) return read_nametable(map_nametable_address(address));
			return cartridge->read_chr(address);
		}
		return read_palette(map_palette_address(address));
	}
	inline void write_memory(uint16_t address, uint8_t value)
	{
		if (address & 0xC000) throw std::runtime_error{ std::string{ "Illegal ppu read at " } + std::to_string(address) };
		if (~address & 0x3F00)
		{
			if (address & 0x2000) return write_nametable(map_nametable_address(address), value);
			return cartridge->write_chr(address, value);
		}
		return write_palette(map_palette_address(address), value);
	}
	inline void h_scroll()
	{
		if (!background_enabled()) return;
		if ((0x001F & v) == 0x001F)
			v ^= 0x041F;
		else
			v = (~0x001F & v) | (0x001F & (v + 0x0001));
	}
	inline void v_scroll()
	{
		if (!background_enabled()) return;
		uint16_t v1 = ~0x7000 & v;
		if ((0x7000 & v) == 0x7000)
		{
			v1 = ~0x03E0 & v1;
			if ((0x03E0 & v) == 0x03E0)
				v = v1;
			else if ((0x03E0 & v) == 0x03A0)
				v = v1 ^ 0x0800;
			else
				v = v1 | (0x03E0 & (v + 0x0020));
		}
		else
			v = v1 | (0x7000 & (v + 0x1000));
	}
	inline void h_update()
	{
		if (!background_enabled()) return;
		v = (~0x041F & v)
			| (0x041F & t);
	}
	inline void v_update()
	{
		if (!background_enabled()) return;
		v = (~0x7BE0 & v)
			| (0x7BE0 & t);
	}
	inline void reload_shift_registers()
	{
		background_shift_register_l = (background_shift_register_l & 0xFF00) | background_tile_l;
		background_shift_register_h = (background_shift_register_h & 0xFF00) | background_tile_h;
		attribute_latch_l			= (attribute_buffer & 0x01);
		attribute_latch_h			= (attribute_buffer & 0x02) >> 1;
	}
	inline uint8_t sprite_height()
	{
		return (0x20 & control_flags) ? 0x10 : 0x08;
	}
	inline void clear_oam_1()
	{
		for (int i = 0; i < 8; i++)
		{
			oam_1[i].id			= 0x40;
			oam_1[i].y			= 0xFF;
			oam_1[i].tile		= 0xFF;
			oam_1[i].attributes = 0xFF;
			oam_1[i].x			= 0xFF;
			oam_1[i].data_l		= 0x00;
			oam_1[i].data_h		= 0x00;
		}
	}
	inline void evaluate_sprites()
	{
		if (line == 261) return;
		for (int i = 0, n = 0; i < 64; i++)
		{
			int sprite_line = line - oam_ram[i << 2];
			if (sprite_line >= 0 && sprite_line < sprite_height())
			{
				oam_1[n].id			= i;
				oam_1[n].y			= oam_ram[(i << 2) + 0x0000];
				oam_1[n].tile		= oam_ram[(i << 2) + 0x0001];
				oam_1[n].attributes = oam_ram[(i << 2) + 0x0002];
				oam_1[n].x			= oam_ram[(i << 2) + 0x0003];
				if (n++ == 8)
				{
					status_flags |= 0x20;
					return;
				}
			}
		}
	}
	inline void load_sprites()
	{
		for (int i = 0; i < 8; i++)
		{
			oam_0[i]		 = oam_1[i];
			uint16_t address = (sprite_height() == 0x10)
								   ? (0x1000 & (oam_0[i].tile << 12)) | (0x0FE0 & (oam_0[i].tile << 4))
								   : (0x1000 & (control_flags << 9)) | ((oam_0[i].tile << 4));

			uint8_t sprite_line = (line - oam_0[i].y) % sprite_height();
			if (oam_0[i].attributes & 0x80) sprite_line ^= sprite_height() - 0x01;
			address += sprite_line + (sprite_line & 0x08);
			oam_0[i].data_l = read_memory(address + 0);
			oam_0[i].data_h = read_memory(address + 8);
		}
	}
	inline void pixel()
	{
		uint8_t palette		 = 0;
		uint8_t obj_palette  = 0;
		uint8_t obj_priority = 0;
		int16_t pxx			 = dot - 2;

		if ((0x08 & mask_flags) && ((0x02 & mask_flags) || pxx >= 8))
		{
			palette = (NTH_BIT(background_shift_register_h, 15 - x) << 1) | NTH_BIT(background_shift_register_l, 15 - x);
			if (palette) palette |= ((NTH_BIT(attribute_shift_register_h, 7 - x) << 1) | NTH_BIT(attribute_shift_register_l, 7 - x)) << 2;
		}
		if ((0x10 & mask_flags) && ((0x04 & mask_flags) || pxx >= 8))
		{
			for (int i = 7; i >= 0; i--)
			{
				if (oam_0[i].id == 64) continue;
				unsigned spx = pxx - oam_0[i].x;
				if (spx >= 8) continue;
				if (oam_0[i].attributes & 0x40) spx ^= 7;
				uint8_t spr_palette = (NTH_BIT(oam_0[i].data_h, 7 - spx) << 1) | NTH_BIT(oam_0[i].data_l, 7 - spx);
				if (spr_palette == 0) continue;	// Transparent pixel.
				if (oam_0[i].id == 0 && palette && pxx != 255) status_flags |= 0x40;
				spr_palette |= (oam_0[i].attributes & 3) << 2;
				obj_palette  = spr_palette + 16;
				obj_priority = oam_0[i].attributes & 0x20;
			}
		}
		if (obj_palette && (palette == 0 || obj_priority == 0)) palette = obj_palette;
		framebuffer[line * 256 + pxx] = nes_palette[read_memory(0x3F00 + (background_enabled() ? palette : 0))];

		background_shift_register_l <<= 1;
		background_shift_register_h <<= 1;
		attribute_shift_register_l = (attribute_shift_register_l << 1) | attribute_latch_l;
		attribute_shift_register_h = (attribute_shift_register_h << 1) | attribute_latch_h;
	}

	void dot_340()
	{
		name_buffer = read_memory(address_buffer);
		if (line == 239)
			dot_process = &dot_240_0;
		else
			dot_process = &dot_0_0;
	}
	void dot_339()
	{
		address_buffer = nametable_address();
		dot_process	= &dot_340;
	}
	void dot_338()
	{
		name_buffer = read_memory(address_buffer);
		dot_process = &dot_339;
	}
	void dot_321()
	{
		load_sprites();
		address_buffer = nametable_address();
		dot_process	= &dot_nt_even;
	}
	void dot_0_258()
	{
		oam_address = 0;
		if (dot == 320) dot_process = &dot_321;
	}
	void dot_261_280()
	{
		oam_address = 0;
		v_update();
		if (dot == 304) dot_process = &dot_0_258;
	}
	void dot_261_258()
	{
		oam_address = 0;
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
		background_tile_h = read_memory(address_buffer);
		v_scroll();
		if (line == 261)
			dot_process = &dot_261_257;
		else
			dot_process = &dot_0_257;
	}
	void dot_bgh_even()
	{
		pixel();
		background_tile_h = read_memory(address_buffer);
		h_scroll();
		dot_process = &dot_nt_odd;
	}
	void dot_bgh_odd()
	{
		pixel();
		address_buffer += 8;
		if (dot == 255)
			dot_process = &dot_256;
		else
			dot_process = &dot_bgh_even;
	}
	void dot_bgl_even()
	{
		pixel();
		background_tile_l = read_memory(address_buffer);
		dot_process		  = &dot_bgh_odd;
	}
	void dot_bgl_odd()
	{
		pixel();
		address_buffer = background_address();
		dot_process	= &dot_bgl_even;
	}
	void dot_at_even()
	{
		pixel();
		attribute_buffer = read_memory(address_buffer);
		if (v & 0x02) attribute_buffer >>= 2;
		if (v & 0x40) attribute_buffer >>= 4;
		dot_process = &dot_bgl_odd;
	}
	void dot_at_odd()
	{
		pixel();
		address_buffer = attributes_address();
		dot_process	= &dot_at_even;
	}
	void dot_nt_even()
	{
		pixel();
		name_buffer = read_memory(address_buffer);
		dot_process = &dot_at_odd;
	}
	void dot_nt_odd()
	{
		pixel();
		address_buffer = nametable_address();
		reload_shift_registers();
		if (dot == 337)
			dot_process = &dot_338;
		else
			dot_process = &dot_nt_even;
	}
	void dot_261_1()
	{
		status_flags &= ~0x60;
		address_buffer = nametable_address();
		status_flags &= ~0x80;
		dot_process = &dot_nt_even;
	}
	void dot_241_2()
	{
		if (dot == 0 && line == 261) dot_process = &dot_261_1;
	}
	void dot_241_1()
	{
		status_flags |= 0x80;
		if (control_flags & 0x80) nmi_flipflop->set();
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
		clear_oam_1();
		address_buffer = nametable_address();
		dot_process	= &dot_nt_even;
	}
	void dot_0_0()
	{
		if (line == 0 && background_enabled() && odd_frame)
		{
			++dot;
			dot_0_1();
		}
		else
			dot_process = &dot_0_1;
	}

	void clock()
	{
		dot_process();
		(dot = (dot + 1) % 341) || (line = (line + 1) % 262) || (odd_frame = !odd_frame);
	}
	void write(uint8_t index, uint8_t value)
	{
		data_latch = value;

		switch (index)
		{
		case PPUCTRL:
			control_flags = value;
			t			  = (~0x0C00 & t) | (0x0C00 & (control_flags << 10));
			break;
		case PPUMASK:
			mask_flags = value;
			break;
		case OAMADDR:
			oam_address = value;
			break;
		case OAMDATA:
			oam_ram[oam_address] = value;
			oam_address			 = (oam_address + 1) % 0x0100;
			break;
		case PPUSCROLL:
			if ((w = !w))
			{
				x = 0x07 & value;
				t = (~0x001F & t) | (0x001F & (value >> 3));
			}
			else
			{
				t = (~(0x7000 | 0x03E0) & t) | (0x7000 & (value << 12)) | (0x03E0 & (value << 2));
			}
			break;
		case PPUADDR:
			if ((w = !w))
			{
				t = (~0x3F00 & t) | (0x3F00 & (value << 8));
			}
			else
			{
				t = (~0x00FF & t) | (0x00FF & value);
				v = t;
			}
			break;
		case PPUDATA:
			write_memory(0x3FFF & v, value);
			v = (~0x3FFF & v) | (0x3FFF & (v + ((0x04 & control_flags) ? 0x0020 : 0x0001)));
		}
	}
	uint8_t read(uint8_t index)
	{
		switch (index)
		{
		case PPUSTATUS:
			data_latch = (0x1F & data_latch) | status_flags;
			status_flags &= ~0x80;
			w = 0;
			break;
		case OAMDATA:
			data_latch = oam_ram[oam_address];
			break;
		case PPUDATA:
			if (~v & 0x3F00)
			{
				data_latch  = data_buffer;
				data_buffer = read_memory(0x3FFF & v);
			}
			else
			{
				data_latch  = read_memory(0x3FFF & v);
				data_buffer = read_memory(0x2FFF & v);
			}
			v = (~0x3FFF & v) | (0x3FFF & (v + ((0x04 & control_flags) ? 0x0020 : 0x0001)));
		}
		return data_latch;
	}
	void reset()
	{
		odd_frame	 = false;
		line		  = 0;
		dot_process   = dot_0_0;
		dot			  = 0;
		control_flags = 0;
		mask_flags	= 0;
		status_flags  = 0;

		framebuffer.fill(0);
		nametable_ram.fill(0xFF);
		oam_ram.fill(0);
	}
}
