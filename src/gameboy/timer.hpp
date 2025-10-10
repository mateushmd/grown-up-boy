#pragma once

#include <cstdint>

#include "event.hpp"

namespace gameboy
{
constexpr int DIV_TARGET = 256;

class Timer
{
  private:
    unsigned int timaCounter;

    bool stop;

    uint8_t oldTmaValue;

    uint16_t &DIV;
    uint8_t &TIMA, &TMA, &TAC;

    bool timaOverflow;

    unsigned int getFrequency();

  public:
    util::Event<> onTimaOverflow;
    util::Event<uint16_t, uint8_t> onUpdate;

    Timer(uint16_t &, uint8_t &, uint8_t &, uint8_t &);

    void update(const uint8_t);
};
} // namespace gameboy
