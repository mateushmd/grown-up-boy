#pragma once

#include <memory>
#include <chrono>
#include <cstdint>

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
        std::chrono::duration<double> deltaTime;
        bool running;
        bool cartridgeSet;
        bool bootRomSet;

        bool skipBoot;
        bool cgb;
        bool debug;

        Bus bus;
        CPU cpu;
        std::unique_ptr<Debugger> debugger;

        void update();

    public:
        GameBoy(Options options);
        GameBoy(bool skipBoot, bool cgb, bool debug);

        void setCartridge(std::string path);
        void setBootRom(std::string path);
        void start();
        void mainLoop();
    };
}