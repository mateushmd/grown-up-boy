#pragma once

#include <cstdint>
#include <functional>

namespace emulator {
    class EventQueue {
        private:
            struct Event {
                struct Event *q_next;
                struct Event *s_next;
                uint64_t time;
                std::function<void(void)> callback;
            };

            
        public:
             
    };
}
