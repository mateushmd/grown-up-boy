#pragma once

#include <cstdint>

namespace core {
    class Joypad {
        private: 
            uint8_t joyp;

        public:
            uint8_t read() const;
            void write(const uint8_t value);
    };
}
