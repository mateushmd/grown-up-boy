#pragma once

#include <cstdint>

namespace emulator::io {
    class Interrupts {
        private:
            uint8_t if_;

        public:
            Interrupts();

            uint8_t read() const;
            void write(const uint8_t value);
    };
}
