#include <string>
#include <bitset>
#include <cstdint>

#include "bitwise.hpp"

namespace util::bitwise
{
    std::string getBits(uint8_t value)
    {
        return std::bitset<8>(value).to_string();
    }

    std::string getBits(uint16_t value)
    {
        auto str = std::bitset<16>(value).to_string();
        str.insert(4, " ");
        return str;
    }
}