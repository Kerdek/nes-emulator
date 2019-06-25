#pragma once

#include <cstddef>

namespace sdl2
{
  class sdl2;
  class window;
  class renderer;
  class texture;

  class sdl2
  {
    sdl2(sdl2 const &) = delete;
    sdl2(sdl2 &&) = delete;
    sdl2 & operator=(sdl2 const &) = delete;
    sdl2 & operator=(sdl2 &&) = delete;

  public:
    sdl2();
    ~sdl2();
  };

  class window
  {
    friend class renderer;

    void * p;

    window(window const &) = delete;
    window(window &&) = delete;
    window & operator=(window const &) = delete;
    window & operator=(window &&) = delete;
  public:
    window(char const * title, int width, int height);
    ~window();
  };

  class renderer
  {
    friend class texture;

    void * p;

    renderer(renderer const &) = delete;
    renderer(renderer &&) = delete;
    renderer & operator=(renderer const &) = delete;
    renderer & operator=(renderer &&) = delete;
  public:
    renderer(window const & window);
    ~renderer();

    void set_logical_size(int width, int height);
    void clear();
    void present();
    void copy(texture const & texture);
  };

  class texture
  {
    friend class renderer;

    void * p;

    texture(texture const &) = delete;
    texture(texture &&) = delete;
    texture & operator=(texture const &) = delete;
    texture & operator=(texture &&) = delete;
  public:
    texture(renderer const & renderer, int width, int height);
    ~texture();

    void update(void const * pixels, size_t size);
  };
}
