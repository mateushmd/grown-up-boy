#pragma once

#include <cstdint>
#include <expected>
#include "register.h"

namespace emulator {
    enum class GameBoyError {
        invalid_register,
        unimplemented
    };

    class GameBoy {
        private:
            union RegisterPair af;
            union RegisterPair bc;
            union RegisterPair de;
            union RegisterPair hl;
            uint16_t sp;
            uint16_t pc;

            inline uint8_t get_a();
            inline uint8_t get_f();
            inline uint8_t get_b();
            inline uint8_t get_c();
            inline uint8_t get_d();
            inline uint8_t get_e();
            inline uint8_t get_h();
            inline uint8_t get_l();

            inline void set_a(uint8_t value);
            inline void set_f(uint8_t value);
            inline void set_b(uint8_t value);
            inline void set_c(uint8_t value);
            inline void set_d(uint8_t value);
            inline void set_e(uint8_t value);
            inline void set_h(uint8_t value);
            inline void set_l(uint8_t value);

            std::expected<uint8_t, GameBoyError> get_r8(uint8_t r8);
            std::expected<void, GameBoyError> set_r8(uint8_t r8, uint8_t value);

            std::expected<uint16_t, GameBoyError> get_r16(uint8_t r16);
            std::expected<void, GameBoyError> set_r16(uint8_t r16, uint16_t value);
            
            std::expected<uint16_t, GameBoyError> get_r16stk(uint8_t r16stk);
            std::expected<void, GameBoyError> set_r16stk(uint8_t r16stk, uint16_t value);
            
            std::expected<uint16_t, GameBoyError> get_r16mem(uint8_t r16mem);
            std::expected<void, GameBoyError> set_r16mem(uint8_t r16mem, uint16_t value);
            
            bool get_cond(uint8_t cond);

        public:
    };
}
