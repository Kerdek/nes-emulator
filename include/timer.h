#pragma once

#include <chrono>

namespace nes
{
    class timer
    {
    public:
        timer();
        float elapsed_time();
        void  restart();

    private:
        std::chrono::high_resolution_clock::time_point start;
    };
}
