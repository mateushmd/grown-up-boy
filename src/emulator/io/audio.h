#pragma once

#include <array>
#include <cstdint>

namespace emulator::io {
    class Audio {
        private:
            struct Channel {
                uint8_t nrx1; 
                uint8_t nrx2; 
                uint8_t nrx3; 
                uint8_t nrx4;
            };

            struct FeaturedChannel : Channel {
                uint8_t nrx0;
            };

            uint8_t nr52;
            uint8_t nr51;
            uint8_t nr50;

            FeaturedChannel c1;
            Channel c2;
            FeaturedChannel c3;
            Channel c4;

            std::array<uint8_t, 0xf> wave_pattern_ram;

        public:
            
            uint8_t read(const uint16_t address) const;
            void write(const uint16_t address, const uint8_t value);
    };
}
