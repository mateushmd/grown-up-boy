#include <chrono>

using namespace std::chrono;

namespace emulator
{
    class Time
    {
    private:
        static high_resolution_clock::time_point start;
        static duration<double> _deltaTime;

    public:
        static void captureStartTime();
        static void calculateElapsedTime();
        static double deltaTime();
    };
}