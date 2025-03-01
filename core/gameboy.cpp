#include "gameboy.hpp"
#include "time.hpp"

namespace emulator
{
    void GameBoy::update()
    {
        if (debugger)
            debugger->step();
        auto elapsedCycles = cpu.update();
        timer->update(elapsedCycles);
    }

    GameBoy::GameBoy(Options options)
        : skipBoot(hasFlag(options, Options::SKIP_BOOT)),
          cgb(hasFlag(options, Options::CGB)),
          debug(hasFlag(options, Options::DEBUG)),
          bus(cgb), cpu(bus, skipBoot)
    {
        timer = bus.getTimer();

        if (debug)
            debugger = std::make_unique<Debugger>(cpu, bus);
    }

    GameBoy::GameBoy(bool skipBoot, bool cgb, bool debug)
        : skipBoot(skipBoot), cgb(cgb), debug(debug),
          bus(cgb), cpu(bus, skipBoot)
    {
        timer = bus.getTimer();

        if (debug)
            debugger = std::make_unique<Debugger>(cpu, bus);
    }

    void GameBoy::setCartridge(std::string path)
    {
        if (running)
            return;

        auto cartridge = getCartridge(path);
        bus.setCartridge(cartridge);
        cartridgeSet = true;
    }

    void GameBoy::setBootRom(std::string path)
    {
        if (running || skipBoot)
            return;

        bus.setBootRom(path);
        bootRomSet = true;
    }

    void GameBoy::start()
    {
        if (!cartridgeSet)
            throw std::runtime_error("cartridge was not set");

        if (!skipBoot && !bootRomSet)
            throw std::runtime_error("boot rom was not set");

        running = true;

        mainLoop();
    }

    void GameBoy::mainLoop()
    {
        while (running)
        {
            Time::captureStartTime();
            update();
            Time::calculateElapsedTime();
        }
    }
}