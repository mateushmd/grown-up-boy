#include <cstdint>
#include <expected>
#include "defs.h"
#include "bus.h"

namespace emulator {
    std::expected<uint8_t, GameBoyError> Bus::read(uint16_t address) {
    } 

    std::expected<void, GameBoyError> Bus::write(
        uint16_t address, uint8_t value
    ) {
    }
}
