#pragma once

#include <memory>

#include "logger.hpp"
#include "cpu.hpp"
#include "bus.hpp"
#include "cartridge_factory.hpp"
#include "cartridge.hpp"
#include "debugger.hpp"

using namespace debug;

namespace emulator
{
    using namespace components;

    class GameBoy
    {
    private:
        Bus bus;
        CPU cpu;
        std::unique_ptr<Debugger> debugger;

    public:
        GameBoy(std::string target, bool cgb, bool debug) : bus(cgb), cpu(bus)
        {
            if (debug)
                debugger = std::make_unique<Debugger>(cpu, bus);

            auto cartridge = getCartridge(target);
            bus.setCartridge(cartridge);
        }

        void update()
        {
            if (debugger)
                debugger->step();
            cpu.clock();
        }
    };
}