#pragma once

#include <array>
#include <cstdint>

#include "cartridge.hpp"
#include "audio.h"
#include "joypad.h"
#include "lcd.h"
#include "timer.h"

namespace emulator {
    struct EmulatorContext {
        Cartridge &cartridge;
        std::array<uint8_t, 1024 * 8> &vram;
        std::array<uint8_t, 1024 * 8> &wram;
        std::array<uint8_t, 160> &oam;
        std::array<uint8_t, 127> &hram;
        Audio &audio;
        Joypad &joypad;
        LCD &lcd;
        Timer &timer;
        uint8_t &ie;
        uint8_t &if_register;
        uint8_t &oam_dma_transfer;
    };

    void start();
    void stop();
}
