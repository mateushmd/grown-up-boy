#include "gb.h"

#include <atomic>
#include <array>
#include <cstdint>

#include "cpu.h"
#include "io/audio.h"
#include "io/joypad.h"
#include "io/lcd.h"
#include "io/timer.h"

namespace emulator {
    Cartridge cartridge;
    std::array<uint8_t, 1024 * 8> vram;
    std::array<uint8_t, 1024 * 8> wram;
    std::array<uint8_t, 160> oam;
    std::array<uint8_t, 127> hram;
    io::Audio audio;
    io::Joypad joypad;
    io::LCD lcd;
    io::Timer timer;
    uint8_t ie;
    
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
        ie
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
