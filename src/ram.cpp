#include "ram.h"

namespace nes
{
    void ram::reset()
    {
        std::fill(data.begin(), data.end(), 0);
    }
    uint8_t ram::read(uint16_t address)
    {
        return data[address];
    }
    void ram::write(uint16_t address, uint8_t value)
    {
        data[address] = value;
    }
}
