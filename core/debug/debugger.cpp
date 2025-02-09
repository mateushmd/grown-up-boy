#include <iostream>
#include <iomanip>
#include <sstream>
#include <bitset>

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
        curIndentVal = 0;
        counter = 0;
        mode = DebuggerMode::DIRECT;

        cpu.onFetch.subscribe([this](byte fetched, word PC)
                              { this->onFetch(fetched, PC); });
        cpu.onExecute.subscribe([this](byte opcode, word PC)
                                { this->onExecute(opcode, PC); });
    }

    void Debugger::println(std::string string)
    {
        std::string indentation(curIndentVal, '\t');
        logger::debug << indentation << string << std::endl;
    }

    void Debugger::step()
    {
        logger::debug << "\ndebug> ";

        std::string userInput = "";

        std::getline(std::cin, userInput);

        if (userInput.empty())
            return;
        if (userInput == "reg")
        {
            printRegisters();
        }
        else if (userInput == "mem")
        {
            printMemory();
        }
        else if (userInput == "ping")
        {
            logger::debug << "pong" << std::endl;
            step();
        }
    }

    void Debugger::onFetch(const byte fetched, const word PC)
    {
        logger::debug << "FETCH: "
                      << "Fetched Byte: 0x" << std::hex << std::uppercase << std::setfill('0')
                      << std::setw(2) << static_cast<int>(fetched) << " "
                      << "(Binary: " << std::bitset<8>(fetched) << "),\n"
                      << "PC: 0x" << std::hex << std::setw(4) << PC
                      << " (Binary: " << std::bitset<16>(PC) << ")";
    }

    void Debugger::onExecute(const byte opcode, const word PC)
    {
        logger::debug << "EXECUTE: "
                      << "Opcode: 0x" << std::hex << std::uppercase << std::setfill('0')
                      << std::setw(2) << static_cast<int>(opcode) << ",\n"
                      << "PC: 0x" << std::hex << std::setw(4) << PC;
    }

    void Debugger::printRegisters()
    {
        word AF = cpu.getAF();
        word BC = cpu.getBC();
        word DE = cpu.getDE();
        word HL = cpu.getHL();
        word PC = cpu.getPC();
        word SP = cpu.getSP();

        logger::debug << "AF:" << bitwise::getBits(AF) << " " << "(0x" << AF << ")"
                      << "\nBC:" << bitwise::getBits(BC) << " " << "(0x" << BC << ")"
                      << "\nDE:" << bitwise::getBits(DE) << " " << "(0x" << DE << ")"
                      << "\nHL:" << bitwise::getBits(HL) << " " << "(0x" << HL << ")"
                      << "\nPC:" << bitwise::getBits(PC) << " " << "(0x" << PC << ")"
                      << "\nSP:" << bitwise::getBits(SP) << " " << "(0x" << SP << ")\n";
    }

    void Debugger::printMemory()
    {
        const std::vector<byte> &mem = bus.getWram();

        std::ostringstream log;

        for (int i = 0; i < 16; i++)
        {
            log << "   x" << std::hex << i << "    ";
        }

        log << std::endl;

        for (int i = 0; i < 16; i++)
        {
            log << std::hex << i << "x\t";

            for (int j = 0; j < 16; j++)
            {
                log << "   " << bitwise::getBits(mem[(i << 4) | j]) << "    ";
            }

            log << std::endl;
        }

        logger::debug << log.str();
    }
}
