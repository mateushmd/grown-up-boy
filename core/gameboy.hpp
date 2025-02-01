#pragma once

#include "logger.hpp"
#include "cpu.hpp"
#include "bus.hpp"
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

        Debugger *debugger;

        Profile &profile;

        void update()
        {
            debugger->step();
            cpu.clock();
        }

    public:
        GameBoy(Profile &profile) : profile(profile), bus(profile.cgb), cpu(bus)
        {
            logger::message("Instantiating emulator!");
            bus.init();
        }

        void loadTarget()
        {
            
        }

        void attatchDebugger()
        {
            debugger = new Debugger(cpu, bus);
        }

        ~GameBoy()
        {
            delete debugger;
        }
    };
}