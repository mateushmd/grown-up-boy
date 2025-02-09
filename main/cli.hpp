#include <string.h>
#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "profile.hpp"

using namespace emulator;

// TODO: show help
void showHelp()
{
}

// TODO: complete argument paraser
void parseArgs(Profile *profile, int argc, char *argv[])
{
    if (argc == 0 || std::string(argv[0]) == "-h" || std::string(argv[0]) == "--help")
    {
        profile->help = true;
        return;
    }

    for (int i = 1; i < argc - 1; i++)
    {
        int argLen = strlen(argv[i]);

        if (argLen < 2 || argv[i][0] != '-')
            throw std::invalid_argument("unknown argument: " + std::string(argv[i]));

        if (argv[i][1] == '-')
        {
            if (std::string(argv[i]) == "--debug")
                profile->debug = true;
            else if (std::string(argv[i]) == "--test")
                profile->test = true;
        }
        else
        {
            for (int j = 1; j < argLen; j++)
            {
                switch (argv[i][j])
                {
                case 'd':
                    profile->debug = true;
                    break;
                case 't':
                    profile->test = true;
                    break;
                }
            }
        }
    }

    profile->target = std::string(argv[argc - 1]);
}