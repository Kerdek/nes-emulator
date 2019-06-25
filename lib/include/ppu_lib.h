#pragma once

#include <array>

#include "system.h"
#include "display.h"

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

  /* inline uint32_t nes_palette[] =
  { 0x7C7C7C, 0x0000FC, 0x0000BC, 0x4428BC, 0x940084, 0xA80020, 0xA81000, 0x881400,
    0x503000, 0x007800, 0x006800, 0x005800, 0x004058, 0x000000, 0x000000, 0x000000,
    0xBCBCBC, 0x0078F8, 0x0058F8, 0x6844FC, 0xD800CC, 0xE40058, 0xF83800, 0xE45C10,
    0xAC7C00, 0x00B800, 0x00A800, 0x00A844, 0x008888, 0x000000, 0x000000, 0x000000,
    0xF8F8F8, 0x3CBCFC, 0x6888FC, 0x9878F8, 0xF878F8, 0xF85898, 0xF87858, 0xFCA044,
    0xF8B800, 0xB8F818, 0x58D854, 0x58F898, 0x00E8D8, 0x787878, 0x000000, 0x000000,
    0xFCFCFC, 0xA4E4FC, 0xB8B8F8, 0xD8B8F8, 0xF8B8F8, 0xF8A4C0, 0xF0D0B0, 0xFCE0A8,
    0xF8D878, 0xD8F878, 0xB8F8B8, 0xB8F8D8, 0x00FCFC, 0xF8D8F8, 0x000000, 0x000000 };
   */

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

  inline bool rendering() { return mask.bg || mask.spr; }
  inline int spr_height() { return ctrl.sprSz ? 16 : 8; }

  /* Get CIRAM address according to mirroring */
  uint16_t nt_mirror(uint16_t addr)
  {
    using namespace nes::mirroring;
    switch (mirroring_mode)
    {
      case Vertical:    return addr % 0x800;
      case Horizontal:  return ((addr / 2) & 0x400) + (addr % 0x400);
      default:          return addr - 0x2000;
    }
  }
  void set_mirroring(int mode)
  {
    mirroring_mode = mode;
  }

  enum PPU_MEM
  {
    Unknown = -1,
    CHR,
    Nametables,
    Palettes
  };

  int ppu_addressing(const uint16_t addr) {
    auto in_range = [addr](const auto lower, const auto upper) {
      return (addr >= lower) && (addr <= upper);
    };

    if      (in_range(0x0000, 0x1FFF)) { return CHR; }
    else if (in_range(0x2000, 0x3EFF)) { return Nametables; }
    else if (in_range(0x3F00, 0x3FFF)) { return Palettes; }
    else                               { return Unknown; }
  }

  /* Access PPU memory */
  uint8_t rd(uint16_t addr)
  {
    switch (ppu_addressing(addr))
    {
      case CHR:  return system->cartridge.chr_read(addr);  // CHR-ROM/RAM.
      case Nametables:  return ciRam[nt_mirror(addr)];          // Nametables.
      case Palettes:  // Palettes:
        if ((addr & 0x13) == 0x10) addr &= ~0x10;
        return cgRam[addr & 0x1F] & (mask.gray ? 0x30 : 0xFF);
      default: return 0;
    }
  }
  void wr(uint16_t addr, uint8_t v)
  {
    switch (ppu_addressing(addr))
    {
      case CHR:  system->cartridge.chr_write(addr, v); break;  // CHR-ROM/RAM.
      case Nametables:  ciRam[nt_mirror(addr)] = v; break;         // Nametables.
      case Palettes:  // Palettes:
        if ((addr & 0x13) == 0x10) addr &= ~0x10;
        cgRam[addr & 0x1F] = v; break;
    }
  }

  /* Access PPU through registers. */
  template <bool write> uint8_t access(uint16_t index, uint8_t v)
  {
    using namespace nes::memory;

    index = (index % 8) + 0x2000;

    static uint8_t res;      // Result of the operation.
    [[maybe_unused]] static uint8_t buffer;   // VRAM read buffer.
    static bool latch;  // Detect second reading.

    /* Write into register */
    if constexpr (write)
    {
      res = v;

      switch (index)
      {
        case PPUCTRL:  ctrl.r = v; tAddr.nt = ctrl.nt; break;
        case PPUMASK:  mask.r = v; break;
        case OAMADDR:  oamAddr = v; break;
        case OAMDATA:  oamMem[oamAddr++] = v; break;
        case PPUSCROLL:
          if (!latch) { fX = v & 7; tAddr.cX = v >> 3; }      // First write.
          else  { tAddr.fY = v & 7; tAddr.cY = v >> 3; }      // Second write.
          latch = !latch; break;
        case PPUADDR:
          if (!latch) { tAddr.h = v & 0x3F; }                 // First write.
          else        { tAddr.l = v; vAddr.r = tAddr.r; }     // Second write.
          latch = !latch; break;
        case PPUDATA:  wr(vAddr.addr, v); vAddr.addr += ctrl.incr ? 32 : 1;
      }
    }
    /* Read from register */
    else
    {
      switch (index)
      {
        case PPUSTATUS:  res = (res & 0x1F) | status.r; status.vBlank = 0; latch = 0; break;
        case OAMDATA:  res = oamMem[oamAddr]; break;
        case PPUDATA:
          if (vAddr.addr <= 0x3EFF) {
            res    = buffer;
            buffer = rd(vAddr.addr);
          } else {
            res = buffer = rd(vAddr.addr);
          }
          vAddr.addr += ctrl.incr ? 32 : 1;
      }
    }
    return res;
  }
  template uint8_t access<0>(uint16_t, uint8_t);
  template uint8_t access<1>(uint16_t, uint8_t);

  /* Calculate graphics addresses */
  inline uint16_t nt_addr() { return 0x2000 | (vAddr.r & 0xFFF); }
  inline uint16_t at_addr() { return 0x23C0 | (vAddr.nt << 10) | ((vAddr.cY / 4) << 3) | (vAddr.cX / 4); }
  inline uint16_t bg_addr() { return (ctrl.bgTbl * 0x1000) + (nt * 16) + vAddr.fY; }
  /* Increment the scroll by one pixel */
  inline void h_scroll() { if (!rendering()) return; if (vAddr.cX == 31) vAddr.r ^= 0x41F; else vAddr.cX++; }
  inline void v_scroll()
  {
    if (!rendering()) return;
    if (vAddr.fY < 7) vAddr.fY++;
    else {
      vAddr.fY = 0;
      if      (vAddr.cY == 31)   vAddr.cY = 0;
      else if (vAddr.cY == 29) { vAddr.cY = 0; vAddr.nt ^= 0b10; }
      else                       vAddr.cY++;
    }
  }
  /* Copy scrolling data from loopy T to loopy V */
  inline void h_update() { if (!rendering()) return; vAddr.r = (vAddr.r & ~0x041F) | (tAddr.r & 0x041F); }
  inline void v_update() { if (!rendering()) return; vAddr.r = (vAddr.r & ~0x7BE0) | (tAddr.r & 0x7BE0); }
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
      if (line >= 0 && line < spr_height())
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
      if (spr_height() == 16)
        addr = ((oam[i].tile & 1) * 0x1000) + ((oam[i].tile & ~1) * 16);
      else
        addr = ( ctrl.sprTbl      * 0x1000) + ( oam[i].tile       * 16);

      unsigned sprY = (scanline - oam[i].y) % spr_height();  // Line inside the sprite.
      if (oam[i].attr & 0x80) sprY ^= spr_height() - 1;      // Vertical flip.
      addr += sprY + (sprY & 8);  // Select the second tile if on 8x16.

      oam[i].dataL = rd(addr + 0);
      oam[i].dataH = rd(addr + 8);
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

      pixels[scanline*256 + x] = nes_palette[rd(0x3F00 + (rendering() ? palette : 0))];
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
        case 1:  addr  = nt_addr(); reload_shift(); break;
        case 2:  nt    = rd(addr);  break;
        // Attribute:
        case 3:  addr  = at_addr(); break;
        case 4:  at    = rd(addr);  if (vAddr.cY & 2) { at >>= 4; }
                                    if (vAddr.cX & 2) { at >>= 2; } break;
        // Background (low bits):
        case 5:  addr  = bg_addr(); break;
        case 6:  bgL   = rd(addr);  break;
        // Background (high bits):
        case 7:  addr += 8;         break;
        case 0:  bgH   = rd(addr); h_scroll(); break;
      }
    }
    else if (tick == 256)
    {
      pixel(); bgH = rd(addr); v_scroll(); // Vertical bump.
    }
    else if (tick == 257)
    {
      pixel(); reload_shift(); h_update(); // Update horizontal position.
    }

    // No shift reloading:
    else if (tick == 1)                  { addr = nt_addr(); }
    else if (tick == 321 || tick == 339) { addr = nt_addr(); }

    // Nametable fetch instead of attribute:
    else if (tick == 338) { nt = rd(addr); }
    else if (tick == 340) { nt = rd(addr); }

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
          case 1:  addr  = nt_addr(); reload_shift(); break;
          case 2:  nt    = rd(addr);  break;
          // Attribute:
          case 3:  addr  = at_addr(); break;
          case 4:  at    = rd(addr);  if (vAddr.cY & 2) { at >>= 4; }
                                      if (vAddr.cX & 2) { at >>= 2; } break;
          // Background (low bits):
          case 5:  addr  = bg_addr(); break;
          case 6:  bgL   = rd(addr);  break;
          // Background (high bits):
          case 7:  addr += 8;         break;
          case 0:  bgH   = rd(addr); h_scroll(); break;
        }
    }
    else if (tick == 256)        { pixel(); bgH = rd(addr); v_scroll(); }  // Vertical bump.
    else if (tick == 257)        { pixel(); reload_shift(); h_update(); }  // Update horizontal position.
    else if (in_range(280, 304)) { v_update(); } // Update vertical position.

    // No shift reloading:
    else if (tick == 1)                  { addr = nt_addr(); status.vBlank = false; }
    else if (tick == 321 || tick == 339) { addr = nt_addr(); }

    // Nametable fetch instead of attribute:
    else if (tick == 338) { nt = rd(addr); }
    else if (tick == 340) { nt = rd(addr); if (rendering() && frameOdd) { ++tick; } }

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
