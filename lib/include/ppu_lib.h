#pragma once

#include "bitrange.h"
#include "display.h"
#include "system.h"

#include <array>

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#elif defined(__clang__) && !defined(_MSC_VER)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#endif

//
// Original source: https://github.com/AndreaOrru/LaiNES
//

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

inline bool is_rendering(uint8_t mask)
{
	return 0x18 & mask;
}
inline int sprite_height(uint8_t control_register)
{
	return (0x20 & control_register) ? 0x10 : 0x08;
}
inline uint16_t nt_mirror(nes::mirroring::mirroring mode, uint16_t address)
{
	switch (mode)
	{
	case nes::mirroring::Vertical:
		return address & 0x07FF;
	case nes::mirroring::Horizontal:
		return ((address >> 1) & 0x0400) + (address & 0x02FF);
	default:
		return address - 0x2000;
	}
}

inline uint16_t map_palette_address(uint16_t address)
{
	return (address & ~((0x0003 & address) ? 0x0000 : 0x0010)) & 0x001F;
}
inline uint8_t read_palette(uint16_t address, uint8_t mask, std::array<uint8_t, 0x20> & cg_ram)
{
	return ((mask & 0x01) ? 0x18 : 0xFF) & cg_ram[map_palette_address(address)];
}
inline void write_palette(uint16_t address, uint8_t value, std::array<uint8_t, 0x20> & cg_ram)
{
	cg_ram[map_palette_address(address)] = value;
}

inline uint16_t nt_addr(uint16_t address)
{
	return 0x2000 | (address & 0x0FFF);
}
inline uint16_t at_addr(uint16_t address)
{
	return (0x23C0)
		   | (0x0600 & (address))
		   | (0x0038 & (address >> 4))
		   | (0x0007 & (address >> 2));
}
inline uint16_t bg_addr(uint16_t address, uint8_t nt_latch, uint8_t control_register)
{
	return (0x1000 & (control_register << 8))
		   | (0x0FF0 & (nt_latch << 4))
		   | (0x0007 & (address >> 12));
}

inline uint16_t h_scroll(uint16_t address, uint8_t mask)
{
	if (!is_rendering(mask)) return address;
	if ((0x001F & address) == 0x001F) return 0x041F ^ address;
	else return (0xFFE0 & address) | (0x001F & (address + 1));
}
inline uint16_t v_scroll(uint16_t address, uint8_t mask)
{
	if (!is_rendering(mask)) return address;
	uint16_t t1 = ~0x7000 & address;
	if ((0x7000 & address) == 0x7000)
	{
		t1 = ~0x03E0 & t1;
		if ((0x03E0 & address) == 0x03E0) return t1;
		else if ((0x03E0 & address) == 0x03A0) return t1 ^ 0x0800;
		else return t1 | (0x03E0 & (address + 0x0020));
	}
	else return t1 | (0x7000 & (address + 0x1000));
}

inline uint16_t h_update(uint16_t address, uint16_t t_address, uint8_t mask)
{
	if (!is_rendering(mask)) return address;
	return (~0x041F & address)
		 | ( 0x041F & t_address);
}

namespace PPU
{
	struct Sprite
	{
		uint8_t id;		  // Index in OAM.
		uint8_t x;		  // X position.
		uint8_t y;		  // Y position.
		uint8_t tile;	 // Tile index.
		uint8_t attr;	 // Attributes.
		uint8_t dataL;	// Tile data (low).
		uint8_t dataH;	// Tile data (high).
	};
	union Ctrl
	{
		struct
		{
			unsigned nt : 2;		// Nametable ($2000 / $2400 / $2800 / $2C00).
			unsigned incr : 1;		// Address increment (1 / 32).
			unsigned sprTbl : 1;	// Sprite pattern table ($0000 / $1000).
			unsigned bgTbl : 1;		// BG pattern table ($0000 / $1000).
			unsigned sprSz : 1;		// Sprite size (8x8 / 8x16).
			unsigned slave : 1;		// PPU master/slave.
			unsigned nmi : 1;		// Enable NMI.
		};
		uint8_t r;
	};
	union Mask
	{
		struct
		{
			unsigned gray : 1;		 // Grayscale.
			unsigned bgLeft : 1;	 // Show background in leftmost 8 pixels.
			unsigned sprLeft : 1;	// Show sprite in leftmost 8 pixels.
			unsigned bg : 1;		 // Show background.
			unsigned spr : 1;		 // Show sprites.
			unsigned red : 1;		 // Intensify reds.
			unsigned green : 1;		 // Intensify greens.
			unsigned blue : 1;		 // Intensify blues.
		};
		uint8_t r;
	};
	union Status
	{
		struct
		{
			unsigned bus : 5;		// Not significant.
			unsigned sprOvf : 1;	// Sprite overflow.
			unsigned sprHit : 1;	// Sprite 0 Hit.
			unsigned vBlank : 1;	// In VBlank?
		};
		uint8_t r;
	};
	union Addr
	{
		struct
		{
			unsigned cX : 5;	// Coarse X.
			unsigned cY : 5;	// Coarse Y.
			unsigned nt : 2;	// Nametable.
			unsigned fY : 3;	// Fine Y.
		};
		struct
		{
			unsigned l : 8;
			unsigned h : 7;
		};
		unsigned addr : 14;
		unsigned r : 15;
	};

	inline uint32_t nes_palette[] = {
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

	nes::system *		system;
	platform::display * display;
	template<bool write>
	uint8_t access(uint16_t index, uint8_t v = 0);

	int							 mirroring_mode;	// mirroring mode
	std::array<uint8_t, 0x0800>  ci_ram;				// VRAM for nametables
	std::array<uint8_t, 0x20>	cg_ram;				// VRAM for palettes
	std::array<uint8_t, 0x0100>  oamMem;			// VRAM for sprite properties
	std::array<Sprite, 0x08>	 oam;				// sprite buffer
	std::array<Sprite, 0x08>	 secOam;			// sprite buffer
	std::array<uint32_t, 0xF000> pixels;			// video buffer

	Addr	vAddr, tAddr;	// Loopy V, T.
	uint8_t fX;				 // Fine X.
	uint8_t oamAddr;		 // OAM address.

	Ctrl   ctrl;	  // PPUCTRL   ($2000) register.
	Mask   mask;	  // PPUMASK   ($2001) register.
	Status status;	// PPUSTATUS ($2002) register.

	// Background latches:
	uint8_t nt, at, bgL, bgH;
	// Background shift registers:
	uint8_t  atShiftL, atShiftH;
	uint16_t bgShiftL, bgShiftH;
	bool	 atLatchL, atLatchH;

	// Rendering counters:
	int  line, dot;
	bool odd_frame;

	void set_mirroring(int mode)
	{
		mirroring_mode = mode;
	}

	uint8_t read_memory(uint16_t address)
	{
		if (address & 0xC000) throw std::runtime_error{ std::string{ "Illegal ppu read at " } + std::to_string(address) };
		if (~address & 0x3F00)
		{
			if (address & 0x2000) return ci_ram[nt_mirror((nes::mirroring::mirroring)mirroring_mode, address)];
			return system->cartridge.chr_read(address);
		}
		return read_palette(address, mask.r, cg_ram);
	}
	void write_memory(uint16_t address, uint8_t value)
	{
		if (address & 0xC000) throw std::runtime_error{ std::string{ "Illegal ppu read at " } + std::to_string(address) };
		if (~address & 0x3F00)
		{
			if (address & 0x2000) ci_ram[nt_mirror((nes::mirroring::mirroring)mirroring_mode, address)] = value;
			else system->cartridge.chr_write(address, value);
		}
		else write_palette(address, value, cg_ram);
	}

	bool register_latch;

	// Writing any value to any PPU port, even to the nominally read-only PPUSTATUS, will fill this latch.
	// Reading any readable port (PPUSTATUS, OAMDATA, or PPUDATA) also fills the latch with the bits read
	uint8_t register_result;

	void write_register(uint8_t index, uint8_t value)
	{
		register_result = value;

		switch (index)
		{
		case PPUCTRL:
			ctrl.r  = value;
			tAddr.r = (~0x0C00 & tAddr.r) | (0x0C00 & (ctrl.nt << 10));
			break;
		case PPUMASK:
			mask.r = value;
			break;
		case OAMADDR:
			oamAddr = value;
			break;
		case OAMDATA:
			oamMem[oamAddr++] = value;
			break;
		case PPUSCROLL:
			if ((register_latch = !register_latch))
			{
				fX		= value & 0x07;
				tAddr.r = (~0x001F & tAddr.r) | (0x001F & (value >> 3));
			}
			else
			{
				tAddr.r = (~0x73E0 & tAddr.r) | (0x7000 & (value << 12)) | (0x03E0 & (value << 2));
			}
			break;
		case PPUADDR:
			if ((register_latch = !register_latch))
			{
				tAddr.r = (~0x7F00 & tAddr.r) | (0x3F00 & (value << 8));
			}
			else
			{
				vAddr.r = tAddr.r = (~0x00FF & tAddr.r) | (0x00FF & value);
			}
			break;
		case PPUDATA:
			write_memory(0x3FFF & vAddr.r, value);
			vAddr.r = (~0x3FFF & vAddr.r) | (0x3FFF & (vAddr.r + ((0x04 & ctrl.r) ? 0x0020 : 0x0001)));
		}
	}
	uint8_t read_register(uint8_t index)
	{
		switch (index)
		{
		case PPUSTATUS:
			register_result = (register_result & 0x1F) | status.r;
			status.r &= 0x7F;
			register_latch = 0;
			break;
		case OAMDATA:
			register_result = oamMem[oamAddr];
			break;
		case PPUDATA:
		{
			static uint8_t buffer;
			if (~vAddr.r & 0x3F00)
			{
				register_result = buffer;
				buffer			= read_memory(0x3FFF & vAddr.r);
			}
			else
			{
				register_result = read_memory(0x3FFF & vAddr.r);
				buffer          = read_memory(0x2FFF & vAddr.r);
			}
			vAddr.r = (~0x3FFF & vAddr.r) | (0x3FFF & (vAddr.r + ((0x04 & ctrl.r) ? 0x0020 : 0x0001)));
		}
		}
		return register_result;
	}

	/* Copy scrolling data from loopy T to loopy V */
	inline void v_update()
	{
		if (!is_rendering(mask.r)) return;
		vAddr.r = (vAddr.r & ~0x7BE0) | (tAddr.r & 0x7BE0);
	}
	/* Put new data into the shift registers */
	inline void reload_shift()
	{
		bgShiftL = (bgShiftL & 0xFF00) | bgL;
		bgShiftH = (bgShiftH & 0xFF00) | bgH;

		atLatchL = (at & 1);
		atLatchH = (at & 2);
	}

	/* Clear secondary OAM */
	void clear_oam()
	{
		for (int i = 0; i < 8; i++)
		{
			secOam[i].id	= 64;
			secOam[i].y		= 0xFF;
			secOam[i].tile  = 0xFF;
			secOam[i].attr  = 0xFF;
			secOam[i].x		= 0xFF;
			secOam[i].dataL = 0;
			secOam[i].dataH = 0;
		}
	}

	/* Fill secondary OAM with the sprite infos for the next line */
	void eval_sprites()
	{
		int n = 0;
		for (int i = 0; i < 64; i++)
		{
			int current_line = (line == 261 ? -1 : line) - oamMem[(i * 4) + 0];
			// If the sprite is in the line, copy its properties into secondary OAM:
			if (current_line >= 0 && current_line < sprite_height(ctrl.r))
			{
				secOam[n].id   = i;
				secOam[n].y	= oamMem[(i * 4) + 0];
				secOam[n].tile = oamMem[(i * 4) + 1];
				secOam[n].attr = oamMem[(i * 4) + 2];
				secOam[n].x	= oamMem[(i * 4) + 3];
				++n;
				if (n >= 8)
				{
					status.sprOvf = true;
					break;
				}
			}
		}
	}

	/* Load the sprite info into primary OAM and fetch their tile data. */
	void load_sprites()
	{
		uint16_t addr;
		for (int i = 0; i < 8; i++)
		{
			oam[i] = secOam[i];	// Copy secondary OAM into primary.

			// Different address modes depending on the sprite height:
			if (sprite_height(ctrl.r) == 16)
				addr = ((oam[i].tile & 1) * 0x1000) + ((oam[i].tile & ~1) * 16);
			else
				addr = (ctrl.sprTbl * 0x1000) + (oam[i].tile * 16);

			unsigned sprY = (line - oam[i].y) % sprite_height(ctrl.r);	// Line inside the sprite.
			if (oam[i].attr & 0x80) sprY ^= sprite_height(ctrl.r) - 1;		  // Vertical flip.
			addr += sprY + (sprY & 8);										  // Select the second tile if on 8x16.

			oam[i].dataL = read_memory(addr + 0);
			oam[i].dataH = read_memory(addr + 8);
		}
	}

	/* Process a pixel, draw it if it's on screen */
	void pixel()
	{
		uint8_t palette = 0, objPalette = 0;
		bool	objPriority = 0;
		int		x			= dot - 2;

		if (line < 240 && x >= 0 && x < 256)
		{
			if (mask.bg && !(!mask.bgLeft && x < 8))
			{
				// Background:
				palette = (NTH_BIT(bgShiftH, 15 - fX) << 1) | NTH_BIT(bgShiftL, 15 - fX);
				if (palette)
					palette |= ((NTH_BIT(atShiftH, 7 - fX) << 1) | NTH_BIT(atShiftL, 7 - fX)) << 2;
			}
			// Sprites:
			if (mask.spr && !(!mask.sprLeft && x < 8))
			{
				for (int i = 7; i >= 0; i--)
				{
					if (oam[i].id == 64) continue;	// Void entry.
					unsigned sprX = x - oam[i].x;
					if (sprX >= 8) continue;			  // Not in range.
					if (oam[i].attr & 0x40) sprX ^= 7;	// Horizontal flip.

					uint8_t sprPalette = (NTH_BIT(oam[i].dataH, 7 - sprX) << 1) | NTH_BIT(oam[i].dataL, 7 - sprX);
					if (sprPalette == 0) continue;	// Transparent pixel.

					if (oam[i].id == 0 && palette && x != 255) status.sprHit = true;

					sprPalette |= (oam[i].attr & 3) << 2;
					objPalette  = sprPalette + 16;
					objPriority = oam[i].attr & 0x20;
				}
			}
			// Evaluate priority:
			if (objPalette && (palette == 0 || objPriority == 0)) palette = objPalette;

			pixels[line * 256 + x] = nes_palette[read_memory(0x3F00 + (is_rendering(mask.r) ? palette : 0))];
		}
		// Perform background shifts:
		bgShiftL <<= 1;
		bgShiftH <<= 1;
		atShiftL = (atShiftL << 1) | atLatchL;
		atShiftH = (atShiftH << 1) | atLatchH;
	}

	/* Execute a cycle of a line */

	template<>
	void line_cycle<VISIBLE>()
	{
		auto in_range = [](const auto lower, const auto upper) {
			return (dot >= lower) && (dot <= upper);
		};

		static uint16_t addr;

		// Sprites:
		switch (dot)
		{
		case 1:
			clear_oam();
			break;
		case 257:
			eval_sprites();
			break;
		case 321:
			load_sprites();
			break;
		}

		// Background:
		if (in_range(2, 255) || in_range(322, 337))
		{
			pixel();

			switch (dot % 8)
			{
			// Nametable:
			case 1:
				addr = nt_addr(vAddr.r);
				reload_shift();
				break;
			case 2:
				nt = read_memory(addr);
				break;
			// Attribute:
			case 3:
				addr = at_addr(vAddr.r);
				break;
			case 4:
				at = read_memory(addr);
				if (vAddr.cY & 2)
				{
					at >>= 4;
				}
				if (vAddr.cX & 2)
				{
					at >>= 2;
				}
				break;
			// Background (low bits):
			case 5:
				addr = bg_addr(vAddr.r, nt, ctrl.r);
				break;
			case 6:
				bgL = read_memory(addr);
				break;
			// Background (high bits):
			case 7:
				addr += 8;
				break;
			case 0:
				bgH		= read_memory(addr);
				vAddr.r = h_scroll(vAddr.r, mask.r);
				break;
			}
		}
		else if (dot == 256)
		{
			pixel();
			bgH		= read_memory(addr);
			vAddr.r = v_scroll(vAddr.r, mask.r);	// Vertical bump.
		}
		else if (dot == 257)
		{
			pixel();
			reload_shift();
			vAddr.r = h_update(vAddr.r, tAddr.r, mask.r);	// Update horizontal position.
		}

		// No shift reloading:
		else if (dot == 1)
		{
			addr = nt_addr(vAddr.r);
		}
		else if (dot == 321 || dot == 339)
		{
			addr = nt_addr(vAddr.r);
		}

		// Nametable fetch instead of attribute:
		else if (dot == 338)
		{
			nt = read_memory(addr);
		}
		else if (dot == 340)
		{
			nt = read_memory(addr);
		}

		// Signal line to mapper:
		// if (dot == 260 && rendering()) bus->line();
	}

	template<>
	void line_cycle<PRE>()
	{
		auto in_range = [](const auto lower, const auto upper) {
			return (dot >= lower) && (dot <= upper);
		};

		if (dot == 0 && is_rendering(mask.r) && odd_frame)
		{
			++dot;
		}

		static uint16_t addr;

		// Sprites:
		switch (dot)
		{
		case 1:
			clear_oam();
			status.sprOvf = status.sprHit = false;
			break;
		case 257:
			eval_sprites();
			break;
		case 321:
			load_sprites();
			break;
		}

		// Background:
		if (in_range(2, 255) || in_range(322, 337))
		{
			pixel();

			switch (dot % 8)
			{
			// Nametable:
			case 1:
				addr = nt_addr(vAddr.r);
				reload_shift();
				break;
			case 2:
				nt = read_memory(addr);
				break;
			// Attribute:
			case 3:
				addr = at_addr(vAddr.r);
				break;
			case 4:
				at = read_memory(addr);
				if (vAddr.cY & 2)
				{
					at >>= 4;
				}
				if (vAddr.cX & 2)
				{
					at >>= 2;
				}
				break;
			// Background (low bits):
			case 5:
				addr = bg_addr(vAddr.r, nt, ctrl.r);
				break;
			case 6:
				bgL = read_memory(addr);
				break;
			// Background (high bits):
			case 7:
				addr += 8;
				break;
			case 0:
				bgH		= read_memory(addr);
				vAddr.r = h_scroll(vAddr.r, mask.r);
				break;
			}
		}
		else if (dot == 256)
		{
			pixel();
			bgH		= read_memory(addr);
			vAddr.r = v_scroll(vAddr.r, mask.r);
		}	// Vertical bump.
		else if (dot == 257)
		{
			pixel();
			reload_shift();
			vAddr.r = h_update(vAddr.r, tAddr.r, mask.r);
		}	// Update horizontal position.
		else if (in_range(280, 304))
		{
			v_update();
		}	// Update vertical position.

		// No shift reloading:
		else if (dot == 1)
		{
			addr		  = nt_addr(vAddr.r);
			status.vBlank = false;
		}
		else if (dot == 321 || dot == 339)
		{
			addr = nt_addr(vAddr.r);
		}

		// Nametable fetch instead of attribute:
		else if (dot == 338)
		{
			nt = read_memory(addr);
		}
		else if (dot == 340)
		{
			nt = read_memory(addr);
		}

		// Signal line to mapper:
		// if (dot == 260 && rendering()) bus->line();
	}


	void step()
	{		
		if      (line <  240)             line_cycle<VISIBLE>();
		else if (line == 240 && dot == 0) display->update_frame(pixels.data());
		else if (line == 241 && dot == 1)
		{
			status.r |= 0x80;
			if (ctrl.nmi) system->cpu.set_nmi();
		}
		else if (line == 261) line_cycle<PRE>();

		(dot = (dot + 1) % 341) || (line = (line + 1) % 262) || (odd_frame = !odd_frame);
	}

	void reset()
	{
		odd_frame = false;
		line = 0;
		dot	 = 0;
		ctrl.r   = 0;
		mask.r   = 0;
		status.r = 0;

		pixels.fill(0);
		ci_ram.fill(0xFF);
		oamMem.fill(0);
	}
}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#elif defined(__clang__) && !defined(_MSC_VER)
#pragma clang diagnostic pop
#endif
