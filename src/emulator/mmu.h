#pragma once

#include <array>
#include <cstdint>
#include <expected>
#include "defs.h"

namespace emulator {
    class MMU {
        private:
            std::array<uint8_t, 1024 * 16> mem; 
        public:
            std::expected<uint8_t, GameBoyError> load_byte(uint16_t address);
            std::expected<void, GameBoyError> write_byte(
                uint16_t address, uint8_t value
            );

            std::expected<uint16_t, GameBoyError> load_word(uint16_t address);
            std::expected<void, GameBoyError> write_word(
                uint16_t address, uint16_t value
            );
    };
}
