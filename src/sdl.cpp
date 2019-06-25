#include "sdl.h"
#include "sdlfx.h"

namespace sdl2
{
  sdl2::sdl2()
  {
    SDL_Init(SDL_INIT_VIDEO);
  }
  sdl2::~sdl2()
  {
    SDL_Quit();
  }

  window::window(char const * title, int width, int height)
  {
    p = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        0);
  }
  window::~window()
  {
    SDL_DestroyWindow(static_cast<SDL_Window *>(p));
  }

  renderer::renderer(window const & window)
  {
    p = SDL_CreateRenderer(static_cast<SDL_Window *>(window.p), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  }
  renderer::~renderer()
  {
    SDL_DestroyRenderer(static_cast<SDL_Renderer *>(p));
  }
  void renderer::set_logical_size(int width, int height)
  {
    SDL_RenderSetLogicalSize(static_cast<SDL_Renderer *>(p), width, height);
  }
  void renderer::clear()
  {
    SDL_RenderClear(static_cast<SDL_Renderer *>(p));
  }
  void renderer::present()
  {
    SDL_RenderPresent(static_cast<SDL_Renderer *>(p));
  }
  void renderer::copy(texture const & texture)
  {
    SDL_RenderCopy(static_cast<SDL_Renderer *>(p), static_cast<SDL_Texture *>(texture.p), nullptr, nullptr);
  }

  texture::texture(renderer const & renderer, int width, int height)
  {
    p = SDL_CreateTexture(
          static_cast<SDL_Renderer *>(renderer.p),
          SDL_PIXELFORMAT_RGBA32,
          SDL_TEXTUREACCESS_STREAMING,
          width,
          height);
  }
  texture::~texture()
  {
    SDL_DestroyTexture(static_cast<SDL_Texture *>(p));
  }
  void texture::update(void const * pixels, size_t size)
  {
    SDL_UpdateTexture(static_cast<SDL_Texture *>(p), nullptr, pixels, size);
  }
}
