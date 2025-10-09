#include <chrono>

#include "time.hpp"

using namespace std::chrono;

namespace emulator
{
    high_resolution_clock::time_point Time::start;
    duration<double> Time::_deltaTime;

    void Time::captureStartTime()
    {
        start = high_resolution_clock::now();
    }

    void Time::calculateElapsedTime()
    {
        auto end = high_resolution_clock::now();
        _deltaTime = duration_cast<duration<double>>(end - start);
    }

    double Time::deltaTime()
    {
        return _deltaTime.count();
    }
}