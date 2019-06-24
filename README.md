# nes-emulator

NES emulator written in C++ using SDL2

## Features

- [x] CPU
- [ ] PPU
- [ ] APU
- [x] Input
- [x] Cartridge
- [x] Mapper 0 (NROM)
- [x] Mapper 1 (MMC1)
- [x] Mapper 2 (UxROM)

## Building

Generate the solution files using CMake and build it. It has been tested on Windows and Arch Linux with the following tools and libraries:

- Compilers:
  - GCC 9.1
  - Clang 8.0
  - MSVC 19

- Libraries:
  - SDL2 2.0.9
  - {fmt} 5.3.1

It's recommended to use vcpkg to manage packages on Windows.

## Running

Run the `nes-emulator` executable generated in the `bin` folder. Set the ROM path in `src/main.cpp`, e.g. `cartridge.load("../roms/smb.nes");`

## todo

- Write a PPU from the scratch. I have been using another project's PPU to test my current progress, so I haven't included it here.
- Improve the code and make it easier to select games. Maybe a nice UI with a settings editor?
- Debugger/analyser.
