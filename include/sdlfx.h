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
