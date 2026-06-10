#include "joypad.h"
#include <cstdint>

namespace emulator::devices {
    void Joypad::update() {
        auto selection = (joyp & 0x30) >> 4;

        uint8_t buttons_state = selection & 0b10 ? 0xf : controls & 0xf;
        uint8_t dpad_state = selection & 0b01 ? 0xf : controls >> 4;

        joyp = (joyp & 0xf0) | buttons_state | dpad_state;
    }

    const uint8_t Joypad::read() const {
        return joyp;
    }

    void Joypad::write(const uint8_t value) {
        joyp = (joyp & 0xcf) | (value & 0x30);
        update();
    }
}
