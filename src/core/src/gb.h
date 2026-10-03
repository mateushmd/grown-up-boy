#pragma once

#include <array>
#include <cstdint>

#include "cartridge.hpp"
#include "audio.h"
#include "lcd.h"
#include "timer.h"

namespace core {
    struct EmulatorContext {
        Cartridge &cartridge;
        std::array<uint8_t, 1024 * 8> &vram;
        std::array<uint8_t, 1024 * 8> &wram;
        std::array<uint8_t, 160> &oam;
        std::array<uint8_t, 127> &hram;
        Audio &audio;
        uint8_t &joyp;
        LCD &lcd;
        Timer &timer;
        uint8_t &ie;
        uint8_t &if_register;
        uint8_t &oam_dma_transfer;
    };

    void start();
    void stop();
}
