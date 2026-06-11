#pragma once

#include <cstdint>

namespace emulator::io {
    class LCD {
        private:
            uint8_t lcdc; 
            uint8_t stat;
            uint8_t scy;
            uint8_t scx;
            uint8_t ly;
            uint8_t lyc;
            uint8_t bgp;
            uint8_t obp0;
            uint8_t obp1;
        
        public:

            uint8_t read(const uint16_t address) const;
            void write (const uint16_t address, const uint8_t value);
    };
}
