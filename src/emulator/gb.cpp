#include "gb.h"

#include <atomic>

#include "cpu.h"
#include "io/audio.h"
#include "io/interrupts.h"
#include "io/io_bus.h"
#include "io/joypad.h"
#include "io/lcd.h"
#include "io/timer.h"

namespace emulator {
    io::IOBus io_bus;

    Cartridge cartridge;
    Bus bus(cartridge, io_bus);
    CPU cpu(bus);

    std::atomic<bool> is_running; 

    void start() {
        is_running = true;
    }

    void stop() {
        is_running = false;
    }
}
