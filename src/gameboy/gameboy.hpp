#pragma once

#include <chrono>
#include <cstdint>

#include "cartridge.hpp"
#include "cartridge_factory.hpp"
#include "cpu.hpp"
#include "memory.hpp"

namespace gameboy
{
enum class Options : uint8_t
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

    Memory memory;
    CPU cpu;
    std::unique_ptr<Timer> timer;

    void update();

  public:
    GameBoy(Options options);
    GameBoy(bool skipBoot, bool cgb, bool debug);

    void setCartridge(std::string path);
    void setBootRom(std::string path);
    void start();
    void mainLoop();
};
} // namespace gameboy
