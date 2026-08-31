#pragma once

#include <cstdint>

namespace emulator {
    class Timer {
        private:
            uint16_t counter;
            uint8_t tima;
            uint8_t tma;
            uint8_t tac;

            uint16_t tima_mask;
            bool enable;

        public:
            uint8_t read(uint8_t address) const;
            void write(const uint8_t address, const uint8_t value);

            void update(uint8_t cycles);
    };
}
