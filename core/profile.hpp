#pragma once

#include <string>

namespace emulator
{
    struct Profile
    {
        bool help;

        bool cgb;

        bool test;
        std::string target;

        bool debug;
    };
}