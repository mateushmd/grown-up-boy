#pragma once

#include <cstdint>
#include <expected>
#include <vector>
#include "defs.h"

namespace emulator {
    class Cartridge {
        private:
            std::vector<uint8_t> rom;
            std::vector<uint8_t> ram;

        public:
            std::expected<uint8_t, GameBoyError> read_rom(uint16_t address); 

            std::expected<uint8_t, GameBoyError> read_ram(uint16_t address);
            std::expected<void, GameBoyError> write_ram(
                uint16_t address, uint8_t value
            );
    };
}
