#include <iostream>
#include <iomanip>
#include <sstream>
#include <bitset>

#include "debugger.hpp"
#include "types.hpp"
#include "bitwise.hpp"
#include "cpu.hpp"
#include "bus.hpp"

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
        std::cout << indentation << string << std::endl;
    }

    void Debugger::step()
    {

        std::cout << "\ndebug> ";

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
    }

    void Debugger::onFetch(const byte fetched, const word PC)
    {
        std::ostringstream log;
        log << "FETCH: "
            << "Fetched Byte: 0x" << std::hex << std::uppercase << std::setfill('0')
            << std::setw(2) << static_cast<int>(fetched) << " "
            << "(Binary: " << std::bitset<8>(fetched) << "), "
            << "PC: 0x" << std::hex << std::setw(4) << PC
            << " (Binary: " << std::bitset<16>(PC) << ")";

        println(log.str());
    }

    void Debugger::onExecute(const byte opcode, const word PC)
    {
        std::ostringstream log;
        log << "EXECUTE: "
            << "Opcode: 0x" << std::hex << std::uppercase << std::setfill('0')
            << std::setw(2) << static_cast<int>(opcode) << ", "
            << "PC: 0x" << std::hex << std::setw(4) << PC;

        println(log.str());
    }

    void Debugger::printRegisters()
    {
        word AF = cpu.getAF();
        word BC = cpu.getBC();
        word DE = cpu.getDE();
        word HL = cpu.getHL();
        word PC = cpu.getPC();
        word SP = cpu.getSP();

        std::ostringstream log;
        log << "AF:" << bitwise::getBits(AF) << " " << "(0x" << AF << ")"
            << "\nBC:" << bitwise::getBits(BC) << " " << "(0x" << BC << ")"
            << "\nDE:" << bitwise::getBits(DE) << " " << "(0x" << DE << ")"
            << "\nHL:" << bitwise::getBits(HL) << " " << "(0x" << HL << ")"
            << "\nPC:" << bitwise::getBits(PC) << " " << "(0x" << PC << ")"
            << "\nSP:" << bitwise::getBits(SP) << " " << "(0x" << SP << ")";

        println(log.str());
    }

    void Debugger::printMemory()
    {
        std::vector<byte> mem = bus.getWram(0);

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

        println(log.str());
    }
}
