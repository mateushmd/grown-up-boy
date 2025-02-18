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

    enum class Options : byte
    {
        NONE = 0,
        CGB = 0x1 << 0,
        SKIP_BOOT = 0x1 << 1,
        DEBUG = 0x1 << 2,
    };

    constexpr Options operator|(Options lhs, Options rhs)
    {
        return static_cast<Options>(
            static_cast<std::underlying_type_t<Options>>(lhs) |
            static_cast<std::underlying_type_t<Options>>(rhs));
    }

    constexpr Options operator&(Options lhs, Options rhs)
    {
        return static_cast<Options>(
            static_cast<std::underlying_type_t<Options>>(lhs) &
            static_cast<std::underlying_type_t<Options>>(rhs));
    }

    constexpr bool hasFlag(Options value, Options option)
    {
        return (value & option) == option;
    }

    class GameBoy
    {
    private:
        bool lock;
        bool cartridgeSet;
        bool bootRomSet;

        bool skipBoot;
        bool cgb;
        bool debug;

        Bus bus;
        CPU cpu;
        std::unique_ptr<Debugger> debugger;

        void update()
        {
            if (debugger)
                debugger->step();
            cpu.clock();
        }

    public:
        GameBoy(Options options)
            : skipBoot(hasFlag(options, Options::SKIP_BOOT)),
              cgb(hasFlag(options, Options::CGB)),
              debug(hasFlag(options, Options::DEBUG)),
              bus(cgb), cpu(bus, skipBoot)
        {
            if (debug)
                debugger = std::make_unique<Debugger>(cpu, bus);
        }

        GameBoy(bool skipBoot, bool cgb, bool debug)
            : skipBoot(skipBoot), cgb(cgb), debug(debug),
              bus(cgb), cpu(bus, skipBoot)
        {
            if (debug)
                debugger = std::make_unique<Debugger>(cpu, bus);
        }

        void setCartridge(std::string path)
        {
            if (lock)
                return;

            auto cartridge = getCartridge(path);
            bus.setCartridge(cartridge);
            cartridgeSet = true;
        }

        void setBootRom(std::string path)
        {
            if (lock || skipBoot)
                return;

            bus.setBootRom(path);
            bootRomSet = true;
        }

        void start()
        {
            if (!cartridgeSet)
                throw std::runtime_error("cartridge was not set");

            if (!skipBoot && !bootRomSet)
                throw std::runtime_error("boot rom was not set");

            lock = true;

            while (true)
            {
                update();
            }
        }
    };
}