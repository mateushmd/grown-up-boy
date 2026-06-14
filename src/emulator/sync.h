#pragma once

#include <array>
#include <cstdint>
#include <cstdlib>
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

            std::array<uint32_t, num_modules> last_sync; 
            std::array<uint32_t, num_modules> next_event; 

        public:
            void set_next_event(Module module, uint32_t time);
    };
}
