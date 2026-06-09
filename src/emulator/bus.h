#pragma once

#include <array>
#include <cstdint>
#include <expected>
#include <span>
#include "defs.h"

namespace emulator {
    class Bus {
        private:
            std::array<uint8_t, 1024 * 16> mem; 

            std::expected<std::span<uint8_t> &, GameBoyError> 
                get_device(uint16_t address);

        public:
            std::expected<uint8_t, GameBoyError> load(uint16_t address);
            std::expected<void, GameBoyError> store(
                uint16_t address, uint8_t value
            );
    };
}
