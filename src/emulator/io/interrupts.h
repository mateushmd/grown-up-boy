#pragma once

#include <cstdint>

namespace emulator::io {
    class Interrupts {
        private:
            uint8_t ie;

        public:
            Interrupts();

            const uint8_t read() const;
            void write(const uint8_t value);
    };
}
