#pragma once

#include <cstdint>

namespace emulator {
    enum class GameBoyError {
        invalid_address,
        invalid_cond,
        invalid_flag,
        invalid_register,
        unimplemented
    };

    enum class Flags: uint8_t {
        c = 4,
        h = 5,
        n = 6,
        z = 7
    };

    enum class R8: uint8_t {
        b = 0,
        c = 1,
        d = 2,
        e = 3,
        h = 4,
        l = 5,
        hl = 6,
        a = 7
    };

    enum class R16: uint8_t {
        bc = 0,
        de = 1,
        hl = 2,
        sp = 3
    };

    enum class R16Stk: uint8_t {
        bc = 0,
        de = 1,
        hl = 2,
        af = 3
    };

    enum class R16Mem: uint8_t {
        bc = 0,
        de = 1,
        hl_p = 2,
        hl_m = 3
    };

    enum class Cond: uint8_t {
        nz = 0,  
        z = 1, 
        nc = 2, 
        c = 3
    };
}
