#pragma once

#include <memory>

#include "logger.hpp"
#include "cpu.hpp"
#include "bus.hpp"
#include "cartridge_factory.hpp"
#include "cartridge.hpp"
#include "debugger.hpp"
#include "profile.hpp"

using namespace debug;

namespace emulator
{
    using namespace components;

    class GameBoy
    {
    private:
        CPU cpu;
        Bus bus;

        std::unique_ptr<Debugger> debugger;

        Profile &profile;

        void update()
        {
            debugger->step();
            cpu.clock();
        }

    public:
        GameBoy(Profile &profile) : profile(profile), bus(profile.cgb), cpu(bus)
        {
            if (profile.debug)
            {
                debugger = std::make_unique<Debugger>(cpu, bus);
            }

            auto cartridge = getCartridge(profile.target);
        }
    };
}