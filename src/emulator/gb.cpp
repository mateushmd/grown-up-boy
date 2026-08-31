#include "gb.h"

#include <atomic>
#include <array>
#include <cstdint>

#include "cpu.h"
#include "audio.h"
#include "joypad.h"
#include "lcd.h"
#include "timer.h"

namespace emulator {
    Cartridge cartridge;
    std::array<uint8_t, 1024 * 8> vram;
    std::array<uint8_t, 1024 * 8> wram;
    std::array<uint8_t, 160> oam;
    std::array<uint8_t, 127> hram;
    Audio audio;
    Joypad joypad;
    LCD lcd;
    Timer timer;
    uint8_t ie;
    uint8_t if_register;
    
    struct EmulatorContext context {
        cartridge,
        vram,
        wram,
        oam,
        hram,
        audio,
        joypad,
        lcd,
        timer,
        ie,
        if_register
    };

    Bus bus();
    CPU cpu();

    std::atomic<bool> is_running; 

    void start() {
        is_running = true;
    }

    void stop() {
        is_running = false;
    }
}
