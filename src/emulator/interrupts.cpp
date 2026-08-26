#include <cstdint>

#include "interrupts.h"

namespace emulator {
    Interrupts::Interrupts() : iflag(0xe0) { }

    uint8_t Interrupts::read() const {
        return iflag;
    }

    void Interrupts::write(const uint8_t value) {
        iflag |= value & 0x1f;
    }
}
