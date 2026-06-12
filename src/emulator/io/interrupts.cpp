#include <cstdint>

#include "interrupts.h"

namespace emulator::io {
    Interrupts::Interrupts() : if_(0xe0) { }

    uint8_t Interrupts::read() const {
        return if_;
    }

    void Interrupts::write(const uint8_t value) {
        if_ |= value & 0x1f;
    }
}
