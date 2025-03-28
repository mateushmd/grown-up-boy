#include <utility>

#include "timer.hpp"

namespace emulator
{
    Timer::Timer(Register16 &DIV, Register8 &TIMA, Register8 &TMA, Register8 &TAC)
        : DIV(DIV), TIMA(TIMA), TMA(TMA), TAC(TAC) {}

    unsigned int Timer::getFrequency()
    {
        switch (TAC.get() & 0x3)
        {
        case 0b00:
            return 256;
        case 0b01:
            return 4;
        case 0b10:
            return 16;
        case 0b11:
            return 64;
        default:
            std::unreachable();
        }
    }

    void Timer::update(const byte mCycles)
    {
        if (!stop)
            DIV.set(DIV.get() + mCycles * 4);

        timaCounter += mCycles;

        auto frequency = getFrequency();

        if (timaCounter >= frequency)
        {
            timaCounter -= frequency;

            TIMA.set(oldTmaValue);

            onTimaOverflow.notify();
        }

        oldTmaValue = TMA.get();

        onUpdate.notify(DIV.get(), TIMA.get());
    }
}