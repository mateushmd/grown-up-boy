#include "gb.h"

#include <atomic>

#include "cpu.h"
#include "io/audio.h"
#include "io/interrupts.h"
#include "io/io_dispatcher.h"
#include "io/joypad.h"
#include "io/lcd.h"
#include "io/timer.h"

namespace emulator {
    io::Audio audio;
    io::Interrupts interrupts;
    io::Joypad joypad;
    io::LCD lcd;
    io::Timer timer;

    io::IoDispatcher io_dispatcher(
        audio, interrupts, joypad, lcd, timer
    );

    Cartridge cartridge;
    Bus bus(cartridge, io_dispatcher);
    CPU cpu(bus);

    std::atomic<bool> is_running; 

    void start() {
        is_running = true;
    }

    void stop() {
        is_running = false;
    }
}
