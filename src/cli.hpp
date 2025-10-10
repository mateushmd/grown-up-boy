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

    profile->help = false;
    profile->skipBoot = false;

    for (int i = 1; i < argc - (profile->skipBoot ? 1 : 2); i++)
    {
        int argLen = strlen(argv[i]);

        if (argLen < 2 || argv[i][0] != '-')
            throw std::invalid_argument("unknown argument: " + std::string(argv[i]));

        if (argv[i][1] == '-')
        {
            std::string arg(argv[i]);

            if (arg == "--debug")
                profile->debug = true;
            else if (arg == "--test")
                profile->test = true;
            else if (arg == "--skip-boot")
                profile->skipBoot = true;
            else if (arg == "--help")
                throw std::runtime_error("Invalid usage of --help");
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
                case 's':
                    profile->skipBoot = true;
                    break;
                case 'h':
                    throw std::runtime_error("Invalid usage of -h");
                }
            }
        }
    }

    if (!profile->skipBoot)
        profile->bootRom = std::string(argv[argc - 2]);
    profile->cartridge = std::string(argv[argc - 1]);
}