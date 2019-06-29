#pragma once

#include <array>

#include "system.h"
#include "display.h"
#include "bitrange.h"

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

inline bool rendering(uint8_t mask)
{
  return bit(3, 5).of(mask);
}
inline int spr_height(uint8_t control_register)
{
  return bit(bit(5).of(control_register) ? 4 : 3);
}
inline uint16_t nt_mirror(nes::mirroring::mirroring mode, uint16_t address)
{
  switch (mode)
  {
    case nes::mirroring::Vertical:    return bit(0, 11).of(address);
    case nes::mirroring::Horizontal:  return bit(10).of(address >> 1) + bit(0, 10).of(address);
    default:                          return address - bit(13);
  }
}
enum ppu_map
{
  PPUCTRL = 0x2000,
  PPUMASK = 0x2001,
  PPUSTATUS = 0x2002,
  OAMADDR = 0x2003,
  OAMDATA = 0x2004,
  PPUSCROLL = 0x2005,
  PPUADDR = 0x2006,
  PPUDATA = 0x2007
};
enum PPU_MEM
{
  Unknown = -1,
  CHR,
  Nametables,
  Palettes
};

int ppu_addressing(uint16_t address)
{
  if      (range(0, bit(13)).contains(address))          return CHR;
  else if (range(bit(13), bit(8, 14)).contains(address)) return Nametables;
  else if (range(bit(8, 14), bit(14)).contains(address)) return Palettes;
  else throw std::runtime_error{ "Illegal PPU address (" + std::to_string(address) + ")" };
}
inline uint16_t map_palette_address(uint16_t address)
{
  if (bit(0, 2).is_unset_in(address)) unset(bit(4)).of(address);
  return address;
}
inline uint8_t read_palette(uint16_t address, uint8_t mask, std::array<uint8_t, 0x20> & cg_ram)
{
  return (bit(0).is_set_in(mask) ? bit(4, 6) : bit(0, 8)).of(cg_ram[bit(0, 5).of(map_palette_address(address))]);
}
inline void write_palette(uint16_t address, uint8_t value, std::array<uint8_t, 0x20> & cg_ram)
{
  cg_ram[bit(0, 5).of(map_palette_address(address))] = value;
}

inline uint16_t nt_addr(uint16_t address)
{
  return bit(0, 12).of(address) | bit(13);
}

inline uint16_t at_addr(uint16_t address)
{
  return bit(0, 3).of(address >> 2) | bit(3, 6).of(address >> 4) | bit(6, 10) | bit(10, 12).of(address) | bit(13);
}

inline uint16_t bg_addr(uint16_t address, uint8_t nt_latch, uint8_t control_register)
{
  return
        (0x1000 & (control_register << 8))
      | (0x0FF0 & (nt_latch << 4        ))
      | (0x0007 & (address >> 12        ));
}

inline uint16_t h_scroll(uint16_t address, uint8_t mask)
{
  if (!rendering(mask)) return address;
  if (!bit(0, 5).of(~address)) address ^= (bit(0, 5) | bit(10));
  else address = (address & ~bit(0, 5)) | (bit(0, 5).of(address + 1));
  return address;
}

namespace PPU
{
  struct Sprite
  {
    uint8_t id;     // Index in OAM.
    uint8_t x;      // X position.
    uint8_t y;      // Y position.
    uint8_t tile;   // Tile index.
    uint8_t attr;   // Attributes.
    uint8_t dataL;  // Tile data (low).
    uint8_t dataH;  // Tile data (high).
  };
  union Ctrl
  {
    struct
    {
      unsigned nt : 2;      // Nametable ($2000 / $2400 / $2800 / $2C00).
      unsigned incr : 1;    // Address increment (1 / 32).
      unsigned sprTbl : 1;  // Sprite pattern table ($0000 / $1000).
      unsigned bgTbl : 1;   // BG pattern table ($0000 / $1000).
      unsigned sprSz : 1;   // Sprite size (8x8 / 8x16).
      unsigned slave : 1;   // PPU master/slave.
      unsigned nmi : 1;     // Enable NMI.
    };
    uint8_t r;
  };
  union Mask
  {
    struct
    {
      unsigned gray : 1;     // Grayscale.
      unsigned bgLeft : 1;   // Show background in leftmost 8 pixels.
      unsigned sprLeft : 1;  // Show sprite in leftmost 8 pixels.
      unsigned bg : 1;       // Show background.
      unsigned spr : 1;      // Show sprites.
      unsigned red : 1;      // Intensify reds.
      unsigned green : 1;    // Intensify greens.
      unsigned blue : 1;     // Intensify blues.
    };
    uint8_t r;
  };
  union Status
  {
    struct
    {
      unsigned bus : 5;     // Not significant.
      unsigned sprOvf : 1;  // Sprite overflow.
      unsigned sprHit : 1;  // Sprite 0 Hit.
      unsigned vBlank : 1;  // In VBlank?
    };
    uint8_t r;
  };
  union Addr
  {
    struct
    {
      unsigned cX : 5;  // Coarse X.
      unsigned cY : 5;  // Coarse Y.
      unsigned nt : 2;  // Nametable.
      unsigned fY : 3;  // Fine Y.
    };
    struct
    {
      unsigned l : 8;
      unsigned h : 7;
    };
    unsigned addr : 14;
    unsigned r : 15;
  };

  inline uint32_t nes_palette[] =
  {
    0x7C7C7C, 0xFC0000, 0xBC0000, 0xBC2844, 0x840094, 0x2000A8, 0x0010A8, 0x001488, //
    0x003050, 0x007800, 0x006800, 0x005800, 0x584000, 0x000000, 0x000000, 0x000000, //
    0xBCBCBC, 0xF87800, 0xF85800, 0xFC4468, 0xCC00D8, 0x5800E4, 0x0038F8, 0x105CE4, //
    0x007CAC, 0x00B800, 0x00A800, 0x44A800, 0x888800, 0x000000, 0x000000, 0x000000, //
    0xF8F8F8, 0xFCBC3C, 0xFC8868, 0xF87898, 0xF878F8, 0x9858F8, 0x5878F8, 0x44A0FC, //
    0x00B8F8, 0x18F8B8, 0x54D858, 0x98F858, 0xD8E800, 0x787878, 0x000000, 0x000000, //
    0xFCFCFC, 0xFCE4A4, 0xF8B8B8, 0xF8B8D8, 0xF8B8F8, 0xC0A4F8, 0xB0D0F0, 0xA8E0FC, //
    0x78D8F8, 0x78F8D8, 0xB8F8B8, 0xD8F8B8, 0xFCFC00, 0xF8D8F8, 0x000000, 0x000000  //
  };

  nes::system * system;
  platform::display * display;
  template <bool write> uint8_t access(uint16_t index, uint8_t v = 0);

  int                             mirroring_mode;  // mirroring mode
  std::array<uint8_t, 0x800>      ciRam;      // VRAM for nametables
  std::array<uint8_t, 0x20>       cgRam;      // VRAM for palettes
  std::array<uint8_t, 0x100>      oamMem;     // VRAM for sprite properties
  std::array<Sprite, 8>           oam;        // sprite buffer
  std::array<Sprite, 8>           secOam;     // sprite buffer
  std::array<uint32_t, 256 * 240> pixels;     // video buffer

  Addr    vAddr, tAddr;  // Loopy V, T.
  uint8_t fX;            // Fine X.
  uint8_t oamAddr;       // OAM address.

  Ctrl   ctrl;    // PPUCTRL   ($2000) register.
  Mask   mask;    // PPUMASK   ($2001) register.
  Status status;  // PPUSTATUS ($2002) register.

  // Background latches:
  uint8_t nt, at, bgL, bgH;
  // Background shift registers:
  uint8_t atShiftL, atShiftH; uint16_t bgShiftL, bgShiftH;
  bool atLatchL, atLatchH;

  // Rendering counters:
  int scanline, tick;
  bool frameOdd;

  void set_mirroring(int mode)
  {
    mirroring_mode = mode;
  }

  uint8_t read_memory(uint16_t address)
  {
    switch (ppu_addressing(address))
    {
    case CHR:
      return system->cartridge.chr_read(address);
    case Nametables:
      return ciRam[nt_mirror((nes::mirroring::mirroring)mirroring_mode, address)];
    case Palettes:
      return read_palette(address, mask.r, cgRam);
    default:
      throw std::runtime_error{ "Illegal ppu read (" + std::to_string(address) + ")" };
    }
  }
  void write_memory(uint16_t address, uint8_t value)
  {
    switch (ppu_addressing(address))
    {
    case CHR:
      system->cartridge.chr_write(address, value);
      break;
    case Nametables:
      ciRam[nt_mirror((nes::mirroring::mirroring)mirroring_mode, address)] = value;
      break;
    case Palettes:
      write_palette(address, value, cgRam);
      break;
    }
  }

  bool register_latch;

  // Writing any value to any PPU port, even to the nominally read-only PPUSTATUS, will fill this latch.
  // Reading any readable port (PPUSTATUS, OAMDATA, or PPUDATA) also fills the latch with the bits read
  uint8_t register_result;

  void write_register(uint16_t index, uint8_t value)
  {
    index = bit(0, 3).of(index) | bit(13);

    register_result = value;

    switch (index)
    {
    case PPUCTRL:
      ctrl.r = value;
      tAddr.nt = ctrl.nt;
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
      if ((register_latch = !register_latch)) { fX = value & 7; tAddr.cX = value >> 3; }
      else  { tAddr.fY = value & 7; tAddr.cY = value >> 3; }
      break;
    case PPUADDR:
      if ((register_latch = !register_latch)) { tAddr.h = value & 0x3F; }
      else        { tAddr.l = value; vAddr.r = tAddr.r; }
      break;
    case PPUDATA:
      write_memory(vAddr.addr, value);
      vAddr.addr += ctrl.incr ? 32 : 1;
    }
  }
  uint8_t read_register(uint16_t index)
  {
    index = bit(0, 3).of(index) | bit(13);

    switch (index)
    {
    case PPUSTATUS:
      register_result = (register_result & 0x1F) | status.r;
      status.vBlank = 0;
      register_latch = 0;
      break;
    case OAMDATA:
      register_result = oamMem[oamAddr];
      break;
    case PPUDATA:
    {
      static uint8_t buffer;
        if (vAddr.addr <= 0x3EFF)
        {
          register_result = buffer;
          buffer = read_memory(vAddr.addr);
        }
        else
        {
          register_result = buffer = read_memory(vAddr.addr);
        }
        vAddr.addr += ctrl.incr ? 32 : 1;
    }
    }
    return register_result;
  }

  /* Increment the scroll by one pixel */
  inline void v_scroll()
  {
    if (!rendering(mask.r)) return;
    if (vAddr.fY < 7) vAddr.fY++;
    else {
      vAddr.fY = 0;
      if      (vAddr.cY == 31)   vAddr.cY = 0;
      else if (vAddr.cY == 29) { vAddr.cY = 0; vAddr.nt ^= 0b10; }
      else                       vAddr.cY++;
    }
  }
  /* Copy scrolling data from loopy T to loopy V */
  inline void h_update() { if (!rendering(mask.r)) return; vAddr.r = (vAddr.r & ~0x041F) | (tAddr.r & 0x041F); }
  inline void v_update() { if (!rendering(mask.r)) return; vAddr.r = (vAddr.r & ~0x7BE0) | (tAddr.r & 0x7BE0); }
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
    for (int i = 0; i < 8; i++) {
      secOam[i].id    = 64;
      secOam[i].y     = 0xFF;
      secOam[i].tile  = 0xFF;
      secOam[i].attr  = 0xFF;
      secOam[i].x     = 0xFF;
      secOam[i].dataL = 0;
      secOam[i].dataH = 0;
    }
  }

  /* Fill secondary OAM with the sprite infos for the next scanline */
  void eval_sprites()
  {
    int n = 0;
    for (int i = 0; i < 64; i++)
    {
      int line = (scanline == 261 ? -1 : scanline) - oamMem[(i * 4) + 0];
      // If the sprite is in the scanline, copy its properties into secondary OAM:
      if (line >= 0 && line < spr_height(ctrl.r))
      {
        secOam[n].id   = i;
        secOam[n].y    = oamMem[(i * 4) + 0];
        secOam[n].tile = oamMem[(i * 4) + 1];
        secOam[n].attr = oamMem[(i * 4) + 2];
        secOam[n].x    = oamMem[(i * 4) + 3];
        ++n;
        if (n >= 8) {
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
    for (int i = 0; i < 8; i++) {
      oam[i] = secOam[i];  // Copy secondary OAM into primary.

      // Different address modes depending on the sprite height:
      if (spr_height(ctrl.r) == 16)
        addr = ((oam[i].tile & 1) * 0x1000) + ((oam[i].tile & ~1) * 16);
      else
        addr = ( ctrl.sprTbl      * 0x1000) + ( oam[i].tile       * 16);

      unsigned sprY = (scanline - oam[i].y) % spr_height(ctrl.r);  // Line inside the sprite.
      if (oam[i].attr & 0x80) sprY ^= spr_height(ctrl.r) - 1;      // Vertical flip.
      addr += sprY + (sprY & 8);  // Select the second tile if on 8x16.

      oam[i].dataL = read_memory(addr + 0);
      oam[i].dataH = read_memory(addr + 8);
    }
  }

  /* Process a pixel, draw it if it's on screen */
  void pixel()
  {
    uint8_t palette = 0, objPalette = 0;
    bool objPriority = 0;
    int x = tick - 2;

    if (scanline < 240 && x >= 0 && x < 256)
    {
      if (mask.bg && !(!mask.bgLeft && x < 8))
      {
        // Background:
        palette = (NTH_BIT(bgShiftH, 15 - fX) << 1) |
                   NTH_BIT(bgShiftL, 15 - fX);
        if (palette)
          palette |= ((NTH_BIT(atShiftH,  7 - fX) << 1) |
                       NTH_BIT(atShiftL,  7 - fX))      << 2;
      }
      // Sprites:
      if (mask.spr && !(!mask.sprLeft && x < 8))
      {
        for (int i = 7; i >= 0; i--)
        {
          if (oam[i].id == 64) continue;  // Void entry.
          unsigned sprX = x - oam[i].x;
          if (sprX >= 8) continue;            // Not in range.
          if (oam[i].attr & 0x40) sprX ^= 7;  // Horizontal flip.

          uint8_t sprPalette = (NTH_BIT(oam[i].dataH, 7 - sprX) << 1) |
                                NTH_BIT(oam[i].dataL, 7 - sprX);
          if (sprPalette == 0) continue;  // Transparent pixel.

          if (oam[i].id == 0 && palette && x != 255) status.sprHit = true;

          sprPalette |= (oam[i].attr & 3) << 2;
          objPalette  = sprPalette + 16;
          objPriority = oam[i].attr & 0x20;
        }
      }
      // Evaluate priority:
      if (objPalette && (palette == 0 || objPriority == 0)) palette = objPalette;

      pixels[scanline*256 + x] = nes_palette[read_memory(0x3F00 + (rendering(mask.r) ? palette : 0))];
    }
    // Perform background shifts:
    bgShiftL <<= 1;
    bgShiftH <<= 1;
    atShiftL = (atShiftL << 1) | atLatchL;
    atShiftH = (atShiftH << 1) | atLatchH;
  }

  /* Execute a cycle of a scanline */

  template<> void scanline_cycle<VISIBLE>()
  {
    auto in_range = [](const auto lower, const auto upper) {
      return (tick >= lower) && (tick <= upper);
    };

    static uint16_t addr;

    // Sprites:
    switch (tick)
    {
      case   1: clear_oam();    break;
      case 257: eval_sprites(); break;
      case 321: load_sprites(); break;
    }

    // Background:
    if (in_range(2, 255) || in_range(322, 337))
    {
      pixel();

      switch (tick % 8)
      {
        // Nametable:
        case 1:  addr  = nt_addr(vAddr.r); reload_shift(); break;
        case 2:  nt    = read_memory(addr);  break;
        // Attribute:
        case 3:  addr  = at_addr(vAddr.r); break;
        case 4:  at    = read_memory(addr);  if (vAddr.cY & 2) { at >>= 4; }
                                    if (vAddr.cX & 2) { at >>= 2; } break;
        // Background (low bits):
        case 5:  addr  = bg_addr(vAddr.r, nt, ctrl.r); break;
        case 6:  bgL   = read_memory(addr);  break;
        // Background (high bits):
        case 7:  addr += 8;         break;
        case 0:  bgH   = read_memory(addr); vAddr.r = h_scroll(vAddr.r, mask.r); break;
      }
    }
    else if (tick == 256)
    {
      pixel(); bgH = read_memory(addr); v_scroll(); // Vertical bump.
    }
    else if (tick == 257)
    {
      pixel(); reload_shift(); h_update(); // Update horizontal position.
    }

    // No shift reloading:
    else if (tick == 1)                  { addr = nt_addr(vAddr.r); }
    else if (tick == 321 || tick == 339) { addr = nt_addr(vAddr.r); }

    // Nametable fetch instead of attribute:
    else if (tick == 338) { nt = read_memory(addr); }
    else if (tick == 340) { nt = read_memory(addr); }

    // Signal scanline to mapper:
    // if (tick == 260 && rendering()) bus->scanline();
  }

  template<> void scanline_cycle<POST>()
  {
    if (tick == 0) {
      display->update_frame(pixels.data());
    }
  }

  // THIS FUNCTION NEEDS MOVED IN ORDER TO ELIMNIATE CIRCULARS
  template<> void scanline_cycle<NMI>()
  {
    if (tick == 1) {
      status.vBlank = true;
      if (ctrl.nmi) {
        system->cpu.set_nmi();
     }
   }
  }

  template<> void scanline_cycle<PRE>()
  {
    auto in_range = [](const auto lower, const auto upper) {
      return (tick >= lower) && (tick <= upper);
    };

    static uint16_t addr;

    // Sprites:
    switch (tick) {
      case   1: clear_oam(); status.sprOvf = status.sprHit = false; break;
      case 257: eval_sprites(); break;
      case 321: load_sprites(); break;
    }

    // Background:
    if (in_range(2, 255) || in_range(322, 337)) {
        pixel();

        switch (tick % 8) {
          // Nametable:
          case 1:  addr  = nt_addr(vAddr.r); reload_shift(); break;
          case 2:  nt    = read_memory(addr);  break;
          // Attribute:
          case 3:  addr  = at_addr(vAddr.r); break;
          case 4:  at    = read_memory(addr);  if (vAddr.cY & 2) { at >>= 4; }
                                      if (vAddr.cX & 2) { at >>= 2; } break;
          // Background (low bits):
          case 5:  addr  = bg_addr(vAddr.r, nt, ctrl.r); break;
          case 6:  bgL   = read_memory(addr);  break;
          // Background (high bits):
          case 7:  addr += 8;         break;
          case 0:  bgH   = read_memory(addr); vAddr.r = h_scroll(vAddr.r, mask.r); break;
        }
    }
    else if (tick == 256)        { pixel(); bgH = read_memory(addr); v_scroll(); }  // Vertical bump.
    else if (tick == 257)        { pixel(); reload_shift(); h_update(); }  // Update horizontal position.
    else if (in_range(280, 304)) { v_update(); } // Update vertical position.

    // No shift reloading:
    else if (tick == 1)                  { addr = nt_addr(vAddr.r); status.vBlank = false; }
    else if (tick == 321 || tick == 339) { addr = nt_addr(vAddr.r); }

    // Nametable fetch instead of attribute:
    else if (tick == 338) { nt = read_memory(addr); }
    else if (tick == 340) { nt = read_memory(addr); if (rendering(mask.r) && frameOdd) { ++tick; } }

    // Signal scanline to mapper:
    // if (tick == 260 && rendering()) bus->scanline();
  }

  // THIS FUNCTION NEEDS MOVED IN ORDER TO ELIMINATE CIRCULARS
  /* Execute a PPU cycle. */
  void step()
  {
    switch (scanline)
    {
      case       240:  scanline_cycle<POST>();    break;
      case       241:  scanline_cycle<NMI>();     break;
      case       261:  scanline_cycle<PRE>();     break;
      case 0 ... 239:  scanline_cycle<VISIBLE>(); break;
    }
    // Update tick and scanline counters:
    ++tick;
    if (tick > 340)
    {
      tick %= 341;
      ++scanline;
      if (scanline > 261)
      {
        scanline = 0;
        frameOdd ^= 1;
      }
    }
  }

  void reset()
  {
    frameOdd = false;
    scanline = 0;
    tick = 0;
    ctrl.r = 0;
    mask.r = 0;
    status.r = 0;

    pixels.fill(0);
    ciRam.fill(0xFF);
    oamMem.fill(0);
  }
}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#elif defined(__clang__) && !defined(_MSC_VER)
#pragma clang diagnostic pop
#endif
