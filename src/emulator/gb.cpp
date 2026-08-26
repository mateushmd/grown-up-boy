#include "gb.h"

#include <atomic>

#include "cpu.h"
#include "interrupts.h"
#include "io/audio.h"
#include "io/io_bus.h"
#include "io/joypad.h"
#include "io/lcd.h"
#include "io/timer.h"

namespace emulator {
    Cartridge cartridge;
    Interrupts interrputs;
    Bus bus(cartridge, interrputs);
    CPU cpu(bus);

    std::atomic<bool> is_running; 

    void start() {
        is_running = true;
    }

    void stop() {
        is_running = false;
    }
}
