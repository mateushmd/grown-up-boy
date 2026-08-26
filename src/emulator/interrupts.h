#pragma once

#include <cstdint>

namespace emulator {
    class Interrupts {
        private:
            uint8_t ie;
            uint8_t iflag;

        public:
            Interrupts();

            uint8_t read() const;
            void write(const uint8_t value);
    };
}
