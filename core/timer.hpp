#include "types.hpp"

namespace emulator
{
    constexpr byte DIV_TARGET = 256;

    class Timer
    {
    private:
        unsigned int divCounter;
        unsigned int timaCounter;

        byte *const div, tima;
        const byte *const tma, tac;

        unsigned int getFrequency();

    public:
        Timer(byte *const, byte *const, const byte *const, const byte *const);

        void update(const unsigned int);
    };
}