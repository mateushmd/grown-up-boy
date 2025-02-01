#pragma once

#include <iostream>
#include <string>

namespace logger
{
    inline auto message(std::string message)
    {
        std::cout << message << std::endl;
    }

    inline auto warning(std::string message)
    {
    }

    inline auto error(std::string message)
    {
    }

    inline auto debug(std::string messsage)
    {
    }
}