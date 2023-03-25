#pragma once
#include <cstdint>

namespace gcl {
    enum class Opcodes : uint8_t {
        // misc
        HLT = 0x00,
        BREAK = 0x90,
        CALL = 0x30,
        NCALL = 0x31,

        PUSHS = 0x99 
    };
}  // namespace gcl