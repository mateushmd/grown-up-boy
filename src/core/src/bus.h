#pragma once

#include <cstdint>

#include "gb.h"

namespace core {
    class Bus {
        public:
            Bus();

            uint8_t read(uint16_t address, struct EmulatorContext &context);
            void write(uint16_t address, uint8_t value, struct EmulatorContext &context);
    };
}
