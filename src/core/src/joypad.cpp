#include "joypad.h"

namespace core {
    uint8_t Joypad::read() const {
        return joyp;
    }

    void Joypad::write(const uint8_t value) {
        joyp = (joyp & 0xF) | (value & 0xF0);
    }
}
