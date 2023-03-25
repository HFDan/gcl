#pragma once

#include <bit>
#include <cstdint>

template <typename T>
constexpr auto BigEndianToHostEndian(T number) -> T {
    if constexpr (std::endian::native != std::endian::big) {
        union {
            T x;
            uint8_t u8[sizeof(T)];
        } source, dest;

        source.x = number;

        for (std::size_t i = 0; i < sizeof(T); i++) {
            dest.u8[i] = source.u8[sizeof(T) - i - 1];
        }

        return dest.x;
    }

    return number;
}