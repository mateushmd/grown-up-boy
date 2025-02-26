#include <array>

#include "registers.hpp"
#include "types.hpp"

namespace emulator::components
{

    RegisterPair::RegisterPair(Register8 &high, Register8 &low)
        : hi(high), lo(low) {}

    word RegisterPair::get()
    {
        return (static_cast<word>(hi.get()) << 8) | lo.get();
    }

    void RegisterPair::set(const word rvalue)
    {
        lo.set(static_cast<byte>(rvalue));
        hi.set(rvalue >> 8);
    }

    void RegisterPair::inc()
    {
        word val = get();
        set(++val);
    }

    void RegisterPair::dec()
    {
        word val = get();
        set(--val);
    }
}
