#pragma once

#include <string>

#include "types.hpp"
#include "cpu.hpp"
#include "bus.hpp"

using namespace emulator::components;

namespace debug
{
    enum DebuggerMode
    {
        DIRECT,
        STEP
    };

    enum UserInput
    {

    };

    class Debugger
    {
    private:
        CPU &cpu;
        Bus &bus;

        int curIndentVal;
        int counter;
        std::string indent();
        void println(std::string);
        DebuggerMode mode;

    public:
        Debugger(CPU &, Bus &);
        void step();
        void onFetch(const byte, const word);
        void onExecute(const byte, const word);
        void onHalt();

        void printRegisters();
        void printMemory();

        void waitForUserInput();

        void setMode(const DebuggerMode);
    };
}