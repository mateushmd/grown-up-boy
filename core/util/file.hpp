#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <iterator>
#include <algorithm>
#include <vector>
#include <string>
#include <cstdint>

namespace util::file
{
    std::vector<uint8_t> &loadFile(std::string &filePath, size_t bufferSize)
    {
        std::ifstream file(filePath, std::ios::in | std::ios::binary);

        if (!file)
        {
            throw std::runtime_error("Could not open file");
        }

        uintmax_t fileSize = std::filesystem::file_size(std::filesystem::path(filePath));

        std::vector<uint8_t> buffer;
        buffer.reserve(fileSize);

        uintmax_t bytesRead = 0;

        while (file.read(reinterpret_cast<char *>(buffer.data()), bufferSize))
            ;

        file.close();
    }
}