#pragma once

#include <vector>
#include <string>
#include <cstdint>

namespace util::file
{
    int loadFile(std::string &, std::vector<uint8_t> &);
}