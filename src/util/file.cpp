#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <cstdint>

#include "file.hpp"

namespace util::file
{
    int loadFile(std::string &filePath, std::vector<uint8_t> &buffer)
    {
        std::ifstream file(filePath, std::ios::binary);

        if (!file)
        {
            throw std::runtime_error("Could not open file");
        }

        size_t fileSize = std::filesystem::file_size(std::filesystem::path(filePath));
        buffer.resize(fileSize);

        file.read(reinterpret_cast<char *>(buffer.data()), fileSize);
        file.close();

        return fileSize;
    }
}