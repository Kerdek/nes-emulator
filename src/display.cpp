#include "display.h"

namespace platform
{
  display::display() :
    window{ "nes-emulator", width * 2, height * 2 },
    renderer{ window },
    texture{ renderer, width, height }
  {
    // Bilinear filter
    // SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    renderer.set_logical_size(width, height);
  }

  void display::update_frame(uint32_t const * frame)
  {
    texture.update(frame, width * sizeof(*frame));
  }

  void display::draw()
  {
    renderer.clear();
    renderer.copy(texture);
    renderer.present();
  }
}
