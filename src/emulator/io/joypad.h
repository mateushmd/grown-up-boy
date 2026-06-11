#pragma once

#include <cstdint>

namespace emulator::io {
    class Joypad {
        private:
            uint8_t controls;
            uint8_t joyp;

            void update();
        public:
            const uint8_t read() const;
            void write(const uint8_t value);
    };
}
