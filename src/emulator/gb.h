#pragma once

#include <array>
#include <cstdint>

#include "cartridge.hpp"
#include "io/audio.h"
#include "io/joypad.h"
#include "io/lcd.h"
#include "io/timer.h"

namespace emulator {
    struct EmulatorContext {
        Cartridge &cartridge;
        std::array<uint8_t, 1024 * 8> &vram;
        std::array<uint8_t, 1024 * 8> &wram;
        std::array<uint8_t, 160> &oam;
        std::array<uint8_t, 127> &hram;
        io::Audio &audio;
        io::Joypad &joypad;
        io::LCD &lcd;
        io::Timer &timer;
        uint8_t &ie;
        uint8_t &if_register;
    };

    void start();
    void stop();
}
