#pragma once

#include <cstdint>

namespace emulator {
    union RegisterPair {
        uint16_t pair; 
        struct {
            uint8_t low;
            uint8_t high;
        } regs;
    };
}
