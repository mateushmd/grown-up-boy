#pragma once

#include "joypad.h"
#include "timer.h"
#include "interrupts.h"
#include "audio.h"
#include "lcd.h"
#include <cstdint>

namespace emulator::io {
    class IOBus {
        private:
            Audio audio;
            Interrupts interrupts;
            Joypad joypad;
            LCD lcd;
            Timer timer;

            uint8_t oam_dma_transfer;
            uint8_t boot_rom_mapping_control;
            
        public:
            IOBus();

            uint8_t read(const uint16_t address);
            void write(const uint16_t address, const uint8_t value);

            void update();
    };
}
