#include <iostream>
#include <iomanip>
#include <sstream>
#include <bitset>
#include <format>

#include "debugger.hpp"
#include "types.hpp"
#include "bitwise.hpp"
#include "cpu.hpp"
#include "bus.hpp"
#include "logger.hpp"

using namespace emulator::components;
using namespace util;

namespace debug
{
    Debugger::Debugger(CPU &cpu, Bus &bus) : cpu(cpu),
                                             bus(bus)
    {
        mode = DebuggerMode::DIRECT;

        cpu.onFetch.subscribe([this](byte fetched, word PC)
                              { this->onFetch(fetched, PC); });
        cpu.onExecute.subscribe([this](byte opcode, byte mCycles, word PC)
                                { this->onExecute(opcode, mCycles, PC); });
    }

    void Debugger::step()
    {
        logger::message << "debug";

        if (stepState != StepState::COMPONENTS)
            logger::message << "/" << branch;

        logger::message << "> ";

        std::string userInput = "";
        std::getline(std::cin, userInput);

        if (userInput.empty())
            return;

        parseArgs(userInput);
    }

    void Debugger::onFetch(const byte fetched, const word PC)
    {
        logger::message << "\nFETCH: \n"
                        << "Fetched Byte: 0x" << std::hex << std::uppercase << std::setfill('0')
                        << std::setw(2) << static_cast<int>(fetched)
                        << "(0b" << std::bitset<8>(fetched) << "),\n"
                        << "PC: 0x" << std::hex << std::setw(4) << PC
                        << " (0b" << std::bitset<16>(PC) << ")\n\n";
    }

    void Debugger::onExecute(const byte opcode, const byte mCycles, const word PC)
    {
        logger::message << "\nEXECUTE: \n"
                        << "Opcode: 0x" << std::hex << std::uppercase << std::setfill('0')
                        << std::setw(2) << static_cast<int>(opcode) << " (" << mCycles << "),\n"
                        << "PC: 0x" << std::hex << std::setw(4) << PC << "\n\n";
    }

    void Debugger::printRegisters()
    {
        word PC = cpu.getPC();
        word SP = cpu.getSP();

        logger::message
            << "AF: " << bitwise::getBits(cpu.getA()) << "  " << bitwise::getBits(cpu.getF()) << " " << "(0x" << std::format("{:04X}", cpu.getAF()) << ")\n"
            << "BC: " << bitwise::getBits(cpu.getB()) << "  " << bitwise::getBits(cpu.getC()) << " " << "(0x" << std::format("{:04X}", cpu.getBC()) << ")\n"
            << "DE: " << bitwise::getBits(cpu.getD()) << "  " << bitwise::getBits(cpu.getE()) << " " << "(0x" << std::format("{:04X}", cpu.getDE()) << ")\n"
            << "HL: " << bitwise::getBits(cpu.getH()) << "  " << bitwise::getBits(cpu.getL()) << " " << "(0x" << std::format("{:04X}", cpu.getHL()) << ")\n"
            << "PC: " << bitwise::getBits(PC) << " " << "(0x" << std::format("{:04X}", PC) << ")\n"
            << "SP: " << bitwise::getBits(SP) << " " << "(0x" << std::format("{:04X}", SP) << ")\n";
    }

    void Debugger::parseArgs(std::string args)
    {
        logger::message << '\n';

        if (stepState == StepState::COMPONENTS)
            parseArgsComponents(args);
        else if (stepState == StepState::MEMORY)
            parseArgsMemory(args);

        logger::message << '\n';
        step();
    }

    void Debugger::parseArgsComponents(std::string args)
    {
        try
        {
            if (args == "help")
                showComponentsHelp();
            else if (args == "reg")
                printRegisters();
            else if (args == "brom")
                enterMemoryState("boot rom", bus.getBootRom());
            else if (args == "crom")
                enterMemoryState("cartridge rom", bus.getCartridgeRom());
            else if (args == "vram")
                enterMemoryState("video ram", bus.getVram());
            else if (args == "cram")
                enterMemoryState("cartridge ram", bus.getCartridgeRam());
            else if (args == "wram")
                enterMemoryState("working ram", bus.getWram());
            else if (args == "ioreg")
                enterMemoryState("io registers", bus.getIOReg());
            else if (args == "hram")
                enterMemoryState("high ram", bus.getHram());
            else if (args == "ping")
                logger::message << "pong\n";
            else
            {
                logger::error << "Invalid command";
                showComponentsHelp();
            }
        }
        catch (const std::exception &e)
        {
            logger::error << e.what() << '\n';
            showComponentsHelp();
        }
    }

    void Debugger::enterMemoryState(std::string branch, const std::vector<byte> *mem)
    {
        stepState = StepState::MEMORY;
        this->branch = branch;
        mI.setMemory(mem);
        mI.next();
    }

    void Debugger::parseArgsMemory(std::string rawArgs)
    {
        auto args = splitArgs(rawArgs);

        try
        {
            if (args.size() > 1 && args[0] == "goto")
            {
                int pageNumber = std::stoi(args[1]);
                mI.goToPage(pageNumber);
            }
            else if (args[0] == "help")
                showMemoryHelp();
            else if (args[0] == "next")
            {
                mI.next();
            }
            else if (args[0] == "prev")
            {
                mI.previous();
            }
            else if (args[0] == "quit")
            {
                mI.end();
                stepState = StepState::COMPONENTS;
                branch = "";
            }
            else if (args[0] == "ping")
            {
                logger::message << "pong\n";
            }
            else
            {
                logger::error << "Invalid command";
                showMemoryHelp();
            }
        }
        catch (const std::exception &e)
        {
            logger::error << e.what() << '\n';
        }
    }

    void Debugger::showComponentsHelp()
    {
        std::cout << "Avaliable commands:\n"
                  << "\treg\t\tShow all CPU registers and their values\n"
                  << "\tbrom\t\tNavigate boot ROM memory\n"
                  << "\tcrom\t\tNavigate cartridge ROM memory\n"
                  << "\tvram\t\tNavigate video RAM (VRAM)\n"
                  << "\tcram\t\tNavigate cartridge RAM\n"
                  << "\twram\t\tNavigate working RAM\n"
                  << "\tioreg\t\tNavigate I/O registers\n"
                  << "\thram\t\tNavigate high RAM\n"
                  << "\thelp\t\tList available commands with description\n"
                  << "\tping\t\tEven the devs have no idea what this does\n"
                  << "\nPress Enter to continue emulation.\n";
    }

    void Debugger::showMemoryHelp()
    {
        std::cout << "Available commands: \n"
                  << "\tnext\t\tNavigate to the next page\n"
                  << "\tprev\t\tNavigate to the previous page\n"
                  << "\tgoto <num>\tNavigate to the specified page number\n"
                  << "\tquit\t\tQuit memory navigation mode\n"
                  << "\thelp\t\tList available commands with description\n"
                  << "\tping\t\tEven the devs have no idea what this does\n";
    }

    std::vector<std::string> Debugger::splitArgs(std::string &raw)
    {
        std::istringstream stream(raw);
        std::vector<std::string> args;
        std::string arg;

        while (stream >> arg)
        {
            args.push_back(arg);
        }

        return args;
    }
}
