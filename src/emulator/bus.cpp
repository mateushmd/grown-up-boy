#include <cstdint>
#include <expected>
#include "defs.h"
#include "bus.h"

namespace emulator {
    std::expected<uint8_t, GameBoyError> Bus::load_byte(uint16_t address) {
        if (address >= mem.size()) {
            return std::unexpected(GameBoyError::invalid_address);
        }

        return mem[address];
    } 

    std::expected<void, GameBoyError> Bus::store_byte(
        uint16_t address, uint8_t value
    ) {
        if (address >= mem.size()) {
            return std::unexpected(GameBoyError::invalid_address);
        }

        mem[address] = value;

        return {};
    }

    // TODO: Check behavior on unaligned address
    // TODO: Check behavior on final address
    std::expected<uint16_t, GameBoyError> Bus::load_word(uint16_t address) {
        if (address >= mem.size()) {
            return std::unexpected(GameBoyError::invalid_address);
        }

        return (mem[address + 1] << 8) | mem[address];
    }


    std::expected<void, GameBoyError> Bus::store_word(
        uint16_t address, uint16_t value
    ) {
        if (address >= mem.size()) {
            return std::unexpected(GameBoyError::invalid_address);
        }

        mem[address] = value;
        mem[address + 1] = value >> 8;

        return {};
    }
}
