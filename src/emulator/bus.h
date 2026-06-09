#pragma once

#include <array>
#include <cstdint>
#include <expected>
#include <span>
#include "cartridge.hpp"
#include "defs.h"

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
            std::expected<uint8_t, GameBoyError> read(uint16_t address);
            std::expected<void, GameBoyError> write(
                uint16_t address, uint8_t value
            );
    };
}
