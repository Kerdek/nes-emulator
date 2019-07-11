#include "apu.h"

namespace nes
{
    apu::apu()
    {}

    void apu::reset()
    {}

    uint8_t apu::read(uint16_t)
    {
        return 0;
    }

    void apu::write(uint16_t, uint8_t)
    {}

    void apu::run_frame(int)
    {}
}
