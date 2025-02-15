#pragma once

#include <chrono>

namespace nes {
class timer {
public:
  float elapsed_time();
  void  restart();

private:
  std::chrono::high_resolution_clock::time_point start =
      std::chrono::high_resolution_clock::now();
};
}  // namespace nes