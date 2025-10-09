#pragma once

#include <vector>

#include "types.hpp"

namespace debug
{
    constexpr int PAGEW = 0x10;
    constexpr int PAGEH = 0x10;

    class MemoryInspector
    {
    private:
        const std::vector<byte> *memory = nullptr;
        int page = -1;
        int maxPage = 0;

        void print();

    public:
        void setMemory(const std::vector<byte> *);
        void next();
        void previous();
        void goToPage(int);
        void end();
    };
}