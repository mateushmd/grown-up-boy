#pragma once

#include "types.hpp"
#include "registers.hpp"
#include "event.hpp"

namespace emulator
{
    constexpr int DIV_TARGET = 256;

    class Timer
    {
    private:
        unsigned int timaCounter;

        bool stop;

        byte oldTmaValue;

        Register16 &DIV;
        Register8 &TIMA, &TMA, &TAC;

        bool timaOverflow;

        unsigned int getFrequency();

    public:
        util::Event<> onTimaOverflow;
        util::Event<word, byte> onUpdate;

        Timer(Register16 &, Register8 &, Register8 &, Register8 &);

        void update(const byte);
    };
}