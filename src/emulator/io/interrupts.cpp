#include <cstdint>

#include "interrupts.h"

namespace emulator::io {
    Interrupts::Interrupts() : ie(0xe0) { }

    const uint8_t Interrupts::read() const {
        return ie;
    }

    void Interrupts::write(const uint8_t value) {
        ie |= value & 0x1f;
    }
}
