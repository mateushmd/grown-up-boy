#include "timer.h"

namespace emulator::io {
    /*
     * This method presents undefined behavior when address is invalid and thus
     * should only be used by the bus.
     */
    const uint8_t Timer::read(uint8_t address) const {
        switch (address) {
            case 0: return div;
            case 1: return tima;
            case 2: return tma;
            case 3: return tac;
        }
    }
}
