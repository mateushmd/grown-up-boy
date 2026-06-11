#pragma once

#include <array>
#include <cstdint>
#include "cartridge.hpp"
#include "cartridge.hpp"

namespace emulator {
    class Bus {
        private:
            std::array<uint8_t, 1024 * 8> vram;
            std::array<uint8_t, 1024 * 8> wram;
            std::array<uint8_t, 160> oam;
            std::array<uint8_t, 127> hram;
            uint8_t ie;

            Cartridge &cartridge;

        public:
            uint8_t read(uint16_t address);
            void write(uint16_t address, uint8_t value);
    };
}
