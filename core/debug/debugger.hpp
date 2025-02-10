#pragma once

#include <string>

#include "types.hpp"
#include "cpu.hpp"
#include "bus.hpp"
#include "memory_inspector.hpp"

using namespace emulator::components;

namespace debug
{
    enum DebuggerMode
    {
        DIRECT,
        STEP
    };

    enum StepState
    {
        COMPONENTS,
        MEMORY
    };

    class Debugger
    {
    private:
        CPU &cpu;
        Bus &bus;

        DebuggerMode mode;
        StepState stepState = StepState::COMPONENTS;
        std::string branch = "";

        MemoryInspector mI;

        void parseArgs(std::string);
        void parseArgsComponents(std::string);
        void parseArgsMemory(std::string);

        void enterMemoryState(std::string, const std::vector<byte> *);

        void showComponentsHelp();
        void showMemoryHelp();

        void printRegisters();

        std::vector<std::string> splitArgs(std::string &);

    public:
        Debugger(CPU &, Bus &);
        void step();
        void onFetch(const byte, const word);
        void onExecute(const byte, const word);
        void onHalt();

        void setMode(const DebuggerMode);
    };
}