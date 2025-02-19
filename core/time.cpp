#include <chrono>

#include "time.hpp"

using namespace std::chrono;

namespace emulator
{
    void Time::captureStartTime()
    {
        start = high_resolution_clock::now();
    }

    void Time::calculateElapsedTime()
    {
        auto end = high_resolution_clock::now();
        _deltaTime = duration_cast<duration<double>>(start - end);
    }

    double Time::deltaTime()
    {
        return _deltaTime.count();
    }
}