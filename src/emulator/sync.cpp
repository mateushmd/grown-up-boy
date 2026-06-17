#include <cstdint>
#include <utility>

#include "sync.h"

namespace emulator {
    using Module = Synchronizer::Module;

    Synchronizer::Synchronizer(): earliest_event(INT32_MAX) {
    }

    void Synchronizer::set_next_event(
        Module module, int32_t time
    ) {
        next_event[std::to_underlying(module)] = time;

        if (earliest_event > time)
            earliest_event = time;
    }

    void Synchronizer::rebase() {
        for (size_t i = 0; i < num_modules; i++) {
            last_sync[i] -= earliest_event;
            next_event[i] -= earliest_event;
        }

        earliest_event = 0;
    }
}
