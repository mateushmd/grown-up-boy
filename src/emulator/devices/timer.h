#pragma once

#include <cstdint>

namespace emulator::devices {
    class Timer {
        private:
            uint8_t div;
            uint8_t tima;
            uint8_t tma;
            uint8_t tac;

        public:
            void start_div();
            void stop_div();
            
            const uint8_t read(uint8_t address) const;
            void write(const uint8_t address, const uint8_t value);
    };
}
