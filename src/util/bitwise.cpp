#include <string>
#include <bitset>
#include <cstdint>

#include "bitwise.hpp"

namespace util::bitwise
{
    std::string getBits(uint8_t value)
    {
        auto str = std::bitset<8>(value).to_string();
        str.insert(4, " ");
        return str;
    }

    std::string getBits(uint16_t value)
    {
        auto str = std::bitset<16>(value).to_string();
        str.insert(12, " ");
        str.insert(8, "  ");
        str.insert(4, " ");
        return str;
    }
}