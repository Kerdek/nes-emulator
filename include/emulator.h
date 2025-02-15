#pragma once

#include <memory>

// Keeping this until the next SDL2 version
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpragma-pack"
#endif
// This is needed on Windows, apparently
#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#include "bus.h"
#include "types.h"

namespace SDL2 {
struct Deleter {
  void operator()(SDL_Window* ptr)
  {
    if (ptr) {
      SDL_DestroyWindow(ptr);
    }
  }

  void operator()(SDL_Renderer* ptr)
  {
    if (ptr) {
      SDL_DestroyRenderer(ptr);
    }
  }

  void operator()(SDL_Texture* ptr)
  {
    if (ptr) {
      SDL_DestroyTexture(ptr);
    }
  }
};

using Window   = std::unique_ptr<SDL_Window, Deleter>;
using Renderer = std::unique_ptr<SDL_Renderer, Deleter>;
using Texture  = std::unique_ptr<SDL_Texture, Deleter>;
}  // namespace SDL2

namespace nes {
class emulator {
public:
  emulator();
  ~emulator();

  void close();

  void set_bus(nes::bus&);

  uint8_t get_controller(const size_t) const;
  void    update_frame(const uint32_t*);
  void    draw();

  void run();

private:
  nes::bus* bus = nullptr;

  const unsigned width  = 256;
  const unsigned height = 240;

  SDL2::Window   window;
  SDL2::Renderer renderer;
  SDL2::Texture  texture;
  const uint8_t* keys;

  SDL_Scancode KEY_A[2]      = {SDL_SCANCODE_A, SDL_SCANCODE_ESCAPE};
  SDL_Scancode KEY_B[2]      = {SDL_SCANCODE_S, SDL_SCANCODE_ESCAPE};
  SDL_Scancode KEY_SELECT[2] = {SDL_SCANCODE_BACKSPACE, SDL_SCANCODE_ESCAPE};
  SDL_Scancode KEY_START[2]  = {SDL_SCANCODE_RETURN, SDL_SCANCODE_ESCAPE};
  SDL_Scancode KEY_UP[2]     = {SDL_SCANCODE_UP, SDL_SCANCODE_ESCAPE};
  SDL_Scancode KEY_DOWN[2]   = {SDL_SCANCODE_DOWN, SDL_SCANCODE_ESCAPE};
  SDL_Scancode KEY_LEFT[2]   = {SDL_SCANCODE_LEFT, SDL_SCANCODE_ESCAPE};
  SDL_Scancode KEY_RIGHT[2]  = {SDL_SCANCODE_RIGHT, SDL_SCANCODE_ESCAPE};
};
}  // namespace nes
