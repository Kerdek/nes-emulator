#pragma once

#include <cstring>
#include <type_traits>

namespace nes
{
  template <typename To, typename From>
  To bit_cast(From const & from) noexcept
  {
    static_assert(
        sizeof(To) == sizeof(From), "bit cast requires both sizes to be equal");
    static_assert(
        std::is_trivially_copyable<From>::value,
        "bit cast requires the source type to be trivially copyable");
    static_assert(
        std::is_trivially_copyable<To>::value,
        "bit cast requires the destination type to be trivially copyable");

    To result;
    std::memcpy(&result, &from, sizeof(From));

    return result;
  }
}
