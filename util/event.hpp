#pragma once

#include <vector>
#include <functional>

namespace util
{
    template <typename... Args>
    class Event
    {
    private:
        std::vector<std::function<void(Args...)>> listeners;

    public:
        void subscribe(const std::function<void(Args...)> &listener)
        {
            listeners.push_back(listener);
        }

        void notify(Args... args)
        {
            for (auto &listener : listeners)
            {
                listener(args...);
            }
        }
    };
}