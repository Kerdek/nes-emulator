#include "controller.h"

#include "input.h"

namespace nes
{
    controller::controller(platform::input & input) :
        input{ input }
    {}

    uint8_t controller::read(size_t port)
    {
        if (strobe) return 0x40 | (input.get_controller(port) & 1);    // 1 == A
        uint8_t status = (controller_bits[port] & 1) | 0x40;
        controller_bits[port] >>= 1;
        return status;
    }
    void controller::write(bool signal)
    {
        if (strobe && !signal)
        {
            controller_bits[0] = input.get_controller(0);
            controller_bits[1] = input.get_controller(1);
        }
        strobe = signal;
    }
}
