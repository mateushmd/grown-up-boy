#include <iostream>
#include <iomanip>
#include <sstream>
#include <bitset>
#include <format>
#include <vector>

#include "memory_inspector.hpp"
#include "types.hpp"
#include "bitwise.hpp"
#include "logger.hpp"

using namespace util;

namespace debug
{
    void MemoryInspector::print()
    {
        std::ostringstream log;

        for (byte i = 0; i < PAGEW; i++)
        {
            log << "\tx" << std::format("{:01X}", i);
        }

        log << '\n';

        for (int i = PAGEH * page; i < PAGEH * (page + 1); i++)
        {
            log << std::format("{:04X}", i << 4) << "x";

            for (int j = 0; j < PAGEW; j++)
            {
                int address = (i << 4) | j;

                if (address < memory->size())
                {
                    const byte value = (*memory)[address];
                    log << "\t" << std::format("{:02X}", value);
                }
            }

            log << '\n';
        }

        log << "Page " << std::to_string(page + 1) << "/" << std::to_string(maxPage);

        logger::message << log.str();
    }

    void MemoryInspector::setMemory(const std::vector<byte> *mem)
    {
        memory = mem;

        maxPage = (mem->size() >> 4) / PAGEH;
    }

    void MemoryInspector::next()
    {
        if (page == maxPage - 1)
            throw std::logic_error("The inspector is already at the first page");

        ++page;
        print();
    }

    void MemoryInspector::previous()
    {
        if (page == 0)
            throw std::logic_error("The inspector is already at the last page");

        --page;
        print();
    }

    void MemoryInspector::goToPage(int targetPage)
    {
        if (targetPage > maxPage || targetPage < 1)
            throw std::invalid_argument("The page number is invalid");

        page = targetPage - 1;
        print();
    }

    void MemoryInspector::end()
    {
        page = -1;
        memory = nullptr;
    }
}