#include <cstdint>
#include <expected>
#include "game_boy.h"

namespace emulator {
    inline uint8_t GameBoy::get_a() { return af.regs.high; }
    inline uint8_t GameBoy::get_f() { return af.regs.low; }
    inline uint8_t GameBoy::get_b() { return bc.regs.high; }
    inline uint8_t GameBoy::get_c() { return bc.regs.low; }
    inline uint8_t GameBoy::get_d() { return de.regs.high; }
    inline uint8_t GameBoy::get_e() { return de.regs.low; }
    inline uint8_t GameBoy::get_h() { return hl.regs.high; }
    inline uint8_t GameBoy::get_l() { return hl.regs.low; }

    inline void GameBoy::set_a(uint8_t value) { af.regs.high = value; }
    inline void GameBoy::set_f(uint8_t value) { af.regs.low = value;  }
    inline void GameBoy::set_b(uint8_t value) { bc.regs.high = value; }
    inline void GameBoy::set_c(uint8_t value) { bc.regs.low = value;  }
    inline void GameBoy::set_d(uint8_t value) { de.regs.high = value; }
    inline void GameBoy::set_e(uint8_t value) { de.regs.low = value;  }
    inline void GameBoy::set_h(uint8_t value) { hl.regs.high = value; }
    inline void GameBoy::set_l(uint8_t value) { hl.regs.low = value;  }

    std::expected<uint8_t, GameBoyError> GameBoy::get_r8(uint8_t r8) 
    {
        switch (r8) {
            case 0:
                return get_b();
            case 1:
                return get_c();
            case 2:
                return get_d();
            case 3:
                return get_e();
            case 4:
                return get_h();
            case 5:
                return get_l();
            case 6:
                return std::unexpected(GameBoyError::unimplemented);
            case 7:
                return get_a();
        }

        return std::unexpected(GameBoyError::invalid_register);
    }

    std::expected<void, GameBoyError> GameBoy::set_r8(uint8_t r8, uint8_t value) {
        switch (r8) {
            case 0:
                set_b(value);
                return {};
            case 1:
                set_c(value);
                return {};
            case 2:
                set_d(value);
                return {};
            case 3:
                set_e(value);
                return {};
            case 4:
                set_h(value);
                return {};
            case 5:
                set_l(value);
                return {};
            case 6:
                return std::unexpected(GameBoyError::unimplemented);
            case 7:
                set_a(value);
                return {};
        }

        return std::unexpected(GameBoyError::invalid_register);
    }

    std::expected<uint16_t, GameBoyError> GameBoy::get_r16(uint8_t r16) {
        switch (r16) {
            case 0:
                return bc.pair;
            case 1:
                return de.pair;
            case 2:
                return hl.pair;
            case 3:
                return sp;
        }

        return std::unexpected(GameBoyError::invalid_register);
    }

    std::expected<void, GameBoyError> GameBoy::set_r16(uint8_t r16, uint16_t value) {

        switch (r16) {
            case 0:
                bc.pair = value;
                return {};
            case 1:
                de.pair = value;
                return {};
            case 2:
                hl.pair = value;
                return {};
            case 3:
                sp = value;
                return {};
        }

        return std::unexpected(GameBoyError::invalid_register);
    }

    std::expected<uint16_t, GameBoyError> GameBoy::get_r16stk(uint8_t r16) {
        switch (r16) {
            case 0:
                return bc.pair;
            case 1:
                return de.pair;
            case 2:
                return hl.pair;
            case 3:
                return af.pair;
        }

        return std::unexpected(GameBoyError::invalid_register);
    }

    std::expected<void, GameBoyError> GameBoy::set_r16stk(uint8_t r16, uint16_t value) {

        switch (r16) {
            case 0:
                bc.pair = value;
                return {};
            case 1:
                de.pair = value;
                return {};
            case 2:
                hl.pair = value;
                return {};
            case 3:
                af.pair = value;
                return {};
        }

        return std::unexpected(GameBoyError::invalid_register);
    }

    std::expected<uint16_t, GameBoyError> GameBoy::get_r16mem(uint8_t r16) {
        switch (r16) {
            case 0:
                return bc.pair;
            case 1:
                return de.pair;
            case 2:
                return ++hl.pair;
            case 3:
                return --hl.pair;
        }

        return std::unexpected(GameBoyError::invalid_register);
    }

    std::expected<void, GameBoyError> GameBoy::set_r16mem(uint8_t r16, uint16_t value) {

        switch (r16) {
            case 0:
                bc.pair = value;
                return {};
            case 1:
                de.pair = value;
                return {};
            case 2:
            case 3:
                hl.pair = value;
                return {};
        }

        return std::unexpected(GameBoyError::invalid_register);
    }
}
