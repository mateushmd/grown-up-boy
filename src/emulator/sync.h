#pragma once

#include <cstdlib>
#include <cstdint>
#include <utility>

namespace emulator {
    class Synchronizer {
        public:
            enum class Module: size_t {
                timer,
                num_modules
            };

        private:
            static constexpr size_t num_modules = 
                std::to_underlying(Module::num_modules);

            int32_t last_sync[num_modules]; 
            int32_t next_event[num_modules]; 

            int32_t earliest_event;

        public:
            Synchronizer();

            void set_next_event(Module module, int32_t time);

            void synchronize();

            /**
             *
             * Transforms the earliest incoming event time to become the base 
             * time (0) and readjusts the subsequent events accordingly to 
             * prevent overflows while preserving the timeline.
             *
             */
            void rebase();
    };
}
