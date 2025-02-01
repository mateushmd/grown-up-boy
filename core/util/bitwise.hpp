#pragma once

#include <bitset>
#include "types.hpp"

namespace util::bitwise
{
    inline auto getBits(byte value) -> std::string
    {
        return std::bitset<8>(value).to_string();
    }

    inline auto getBits(word value) -> std::string
    {
        auto str = std::bitset<16>(value).to_string();
        str.insert(4, " ");
        return str;
    }
}