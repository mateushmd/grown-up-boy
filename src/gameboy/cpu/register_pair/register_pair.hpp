#pragma once

#include <cstdint>

namespace gameboy::cpu
{
class RegisterPair
{
  private:
    uint8_t &l;
    uint8_t &r;

  public:
    RegisterPair(uint8_t &l, uint8_t &r) : l(l), r(r) {}

    uint16_t get();
    void set(uint16_t value);
    void inc();
    void dec();
};
} // namespace gameboy::cpu
