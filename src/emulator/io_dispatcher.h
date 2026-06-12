#pragma once

#include "io/joypad.h"
#include "io/timer.h"
#include "io/interrupts.h"
#include "io/audio.h"
#include "io/lcd.h"
#include <cstdint>

namespace emulator {
    class IoDispatcher {
        private:
            io::Joypad joypad;
            io::Timer timer;
            io::Interrupts interrupts;
            io::Audio audio;
            io::LCD lcd;

            uint8_t oam_dma_transfer;
            uint8_t boot_rom_mapping_control;
            
        public:
            uint8_t read(const uint16_t address);
            void write(const uint16_t address, const uint8_t value);
    };
}
