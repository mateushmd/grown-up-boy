#include <utility>

#include "../timer.hpp"

namespace gameboy
{
Timer::Timer(uint16_t &DIV, uint8_t &TIMA, uint8_t &TMA, uint8_t &TAC)
    : DIV(DIV), TIMA(TIMA), TMA(TMA), TAC(TAC)
{
}

unsigned int Timer::getFrequency()
{
    switch (TAC & 0x3)
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

void Timer::update(const uint8_t mCycles)
{
    if (!stop)
        DIV += mCycles * 4;

    timaCounter += mCycles;

    auto frequency = getFrequency();

    if (timaCounter >= frequency)
    {
        timaCounter -= frequency;

        TIMA = oldTmaValue;

        onTimaOverflow.notify();
    }

    oldTmaValue = TMA;

    onUpdate.notify(DIV, TIMA);
}
} // namespace gameboy
