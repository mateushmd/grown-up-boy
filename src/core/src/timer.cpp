#include "timer.h"
#include <cstdint>
#include <utility>

namespace emulator {
    uint8_t Timer::read(uint8_t address) const {
        switch (address) {
            case 0: return (counter & 0xFF00) >> 8;
            case 1: return tima;
            case 2: return tma;
            case 3: return tac;
        }
    }

    void Timer::write(const uint8_t address, const uint8_t value) {
        switch (address) {
            case 0:
                counter = 0;
                break;
            case 1:
                tima = value;
                break;
            case 2:
                tma = value;
                break;
            case 3:
                tac = value;
                enable = tac & 0x4;
                uint8_t clock_select = tac & 0x3;
                if (clock_select == 0) {
                    tima_mask = 0x400;
                } else {
                    tima_mask = 4 << (clock_select << 1);
                }
                break;
        }
    }

    void Timer::update(uint8_t cycles) {
        uint16_t next_counter = counter + cycles;
        
        if (!enable)
            return;

        if (next_counter & tima_mask <= counter & tima_mask)
            return;

        if (tima == 0xFF) {
            tima = tma;
            // TODO: request interrupt
        }

        tima++;
    }
}
