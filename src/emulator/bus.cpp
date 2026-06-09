#include <cstdint>
#include <expected>
#include "defs.h"
#include "bus.h"

namespace emulator {
    std::expected<uint8_t, GameBoyError> Bus::load(uint16_t address) {
        if (address >= mem.size()) {
            return std::unexpected(GameBoyError::invalid_address);
        }

        return mem[address];
    } 

    std::expected<void, GameBoyError> Bus::store(
        uint16_t address, uint8_t value
    ) {
        if (address >= mem.size()) {
            return std::unexpected(GameBoyError::invalid_address);
        }

        mem[address] = value;

        return {};
    }
}
