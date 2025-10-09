#include <cstdint>

#include "../register_pair.hpp"

namespace gameboy::cpu
{
inline uint16_t RegisterPair::get() { return ((int16_t)l << 8) & r; }

inline void RegisterPair::set(uint16_t value)
{
    r = (uint8_t)value;
    l = (uint8_t)(value >> 8);
}

inline void RegisterPair::inc() { set(get() + 1); }
inline void RegisterPair::dec() { set(get() - 1); }
} // namespace gameboy::cpu
