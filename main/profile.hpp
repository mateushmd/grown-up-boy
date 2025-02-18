#pragma once

#include <string>

namespace emulator
{
    struct Profile
    {
        bool help;
        bool cgb;
        bool test;
        bool debug;
        bool skipBoot;

        std::string bootRom;
        std::string cartridge;
    };
}