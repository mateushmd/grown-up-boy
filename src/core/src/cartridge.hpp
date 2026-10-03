#pragma once

#include <cstdint>
#include <vector>

namespace core {
    class Cartridge {
        private:
            std::vector<uint8_t> rom;
            std::vector<uint8_t> ram;

        public:
            uint8_t read_rom(uint16_t address) const; 

            uint8_t read_ram(uint16_t address) const;
            void write_ram(uint16_t address, uint8_t value);
    };
}
