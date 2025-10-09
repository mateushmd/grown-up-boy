#pragma once

#include <iostream>
#include <string>

namespace logger
{
    enum class LogLevel
    {
        MESSAGE,
        WARNING,
        ERROR,
        DEBUG
    };

    class Logger
    {
    private:
        LogLevel level;
        std::ostream &out;

        static constexpr const char *RESET = "\033[0m";
        static constexpr const char *WHITE = "\033[97m";
        static constexpr const char *ORANGE = "\033[33m";
        static constexpr const char *RED = "\033[31m";
        static constexpr const char *BLUE = "\033[34m";

    public:
        Logger(LogLevel lvl, std::ostream &stream = std::cout) : level(lvl), out(stream) {}

        template <typename T>
        Logger &operator<<(const T &msg)
        {
            switch (level)
            {
            case LogLevel::MESSAGE:
                out << WHITE;
                break;
            case LogLevel::WARNING:
                out << ORANGE;
                break;
            case LogLevel::ERROR:
                out << RED;
                break;
            case LogLevel::DEBUG:
                out << BLUE;
                break;
            }
            out << msg << RESET;
            return *this;
        }

        Logger &operator<<(std::ostream &(*manip)(std::ostream &))
        {
            out << RESET << manip;
            return *this;
        }
    };

    inline Logger message(LogLevel::MESSAGE);
    inline Logger warning(LogLevel::WARNING);
    inline Logger error(LogLevel::ERROR);
    inline Logger debug(LogLevel::DEBUG);
}
