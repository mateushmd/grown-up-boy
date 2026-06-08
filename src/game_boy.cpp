#include <cstdint>
#include <expected>
#include "game_boy.h"
#include "defs.h"

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

    inline bool GameBoy::get_flag_z(void) { return get_f() & 8; }
    inline bool GameBoy::get_flag_n(void) { return get_f() & 4; }
    inline bool GameBoy::get_flag_h(void) { return get_f() & 2; }
    inline bool GameBoy::get_flag_c(void) { return get_f() & 1; }

    void GameBoy::set_flag_z(bool value) {
        uint8_t mask = 8;
        auto f_without_flag = get_f() & ~mask;

        set_f((get_f() & ~mask) | (value << 3));
    }

    void GameBoy::set_flag_n(bool value) {
        uint8_t mask = 4;
        auto f_without_flag = get_f() & ~mask;

        set_f((get_f() & ~mask) | (value << 2));
    }

    void GameBoy::set_flag_h(bool value) {
        uint8_t mask = 2;
        auto f_without_flag = get_f() & ~mask;

        set_f((get_f() & ~mask) | (value << 1));
    }

    void GameBoy::set_flag_c(bool value) {
        uint8_t mask = 1;
        auto f_without_flag = get_f() & ~mask;

        set_f((get_f() & ~mask) | value);
    }

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
                return mmu.load_byte(hl.pair);
            case 7:
                return get_a();
        }

        return std::unexpected(GameBoyError::invalid_register);
    }

    std::expected<void, GameBoyError> GameBoy::set_r8(
        uint8_t r8, uint8_t value
    ) {
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
                return mmu.write_byte(hl.pair, value);
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

    std::expected<void, GameBoyError> GameBoy::set_r16(
        uint8_t r16, uint16_t value
    ) {

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

    std::expected<void, GameBoyError> GameBoy::set_r16stk(
        uint8_t r16, uint16_t value
    ) {
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

    std::expected<void, GameBoyError> GameBoy::set_r16mem(
        uint8_t r16, uint16_t value
    ) {

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

    std::expected<bool, GameBoyError> GameBoy::get_cond(uint8_t cond) {
        switch (cond) {
            case 0:
                return !get_flag_z();
            case 1:
                return get_flag_z();
            case 2:
                return !get_flag_c();
            case 3:
                return get_flag_c();
        }

        return std::unexpected(GameBoyError::invalid_cond);
    }

    std::expected<void, GameBoyError> GameBoy::ld_r16_imm16(uint8_t opcode) { 
        uint8_t dest = (opcode >> 4) & 0b11;

        return mmu.load_word(pc)
            .and_then([this, dest](uint16_t imm) {
                return set_r16(dest, imm); 
            })
            .transform([this]() {
                pc += 2;
            });
    }

    std::expected<void, GameBoyError> GameBoy::ld_r16mem_a(uint8_t opcode) { 
        uint8_t dest = (opcode >> 4) & 0b11;

        return get_r16mem(dest)
            .and_then([this](uint16_t r16mem) {
                return mmu.write_byte(
                    r16mem, 
                    get_a()
                );
            });
    }

    std::expected<void, GameBoyError> GameBoy::ld_a_r16mem(uint8_t opcode) { 
        uint8_t source = (opcode >> 4) & 0b11;

        return get_r16mem(source)
            .and_then([this](uint16_t r16mem) {
                return mmu.load_byte(r16mem);
            })
            .transform([this](uint8_t value) {
                set_a(value);
            });
    }

    std::expected<void, GameBoyError> GameBoy::ld_imm16_sp(uint8_t opcode) { 
        return mmu.load_word(pc)
            .and_then([this](uint16_t imm16) {
                return mmu.write_word(imm16, sp);
            });
    }

    std::expected<void, GameBoyError> GameBoy::inc_r16(uint8_t opcode) { 
        uint8_t operand = (opcode >> 4) & 0b11;
        
        return get_r16(operand)
            .and_then([this, operand](uint16_t r16) {
                return set_r16(operand, r16 + 1);
            });
    }

    std::expected<void, GameBoyError> GameBoy::dec_r16(uint8_t opcode) { 
        uint8_t operand = (opcode >> 4) & 0b11;
        
        return get_r16(operand)
            .and_then([this, operand](uint16_t r16) {
                return set_r16(operand, r16 - 1);
            });
    }

    std::expected<void, GameBoyError> GameBoy::add_hl_r16(uint8_t opcode) {
        uint8_t operand = (opcode >> 4) & 0b11;
        
        return get_r16(operand)
            .transform([this](uint16_t r16) {
                auto hl_value = hl.pair;
                hl.pair += r16;
                set_flag_n(false);
                set_flag_h((hl_value & 0xFF) > (hl.pair & 0xFF));
                set_flag_c(hl_value > hl.pair);
            });
    }

    std::expected<void, GameBoyError> GameBoy::inc_r8(uint8_t opcode) { 
        uint8_t operand = (opcode >> 4) & 0b11;
        
        return get_r8(operand)
            .transform([this, operand](uint8_t r8) {
                auto next_value = r8 + 1;
                *set_r8(operand, next_value);
                set_flag_z(next_value == 0);
                set_flag_n(false);
                set_flag_h((r8 & 0xF) > (next_value & 0xF));
            });
    }

    std::expected<void, GameBoyError> GameBoy::dec_r8(uint8_t opcode) {
        uint8_t operand = (opcode >> 4) & 0b11;
        
        return get_r8(operand)
            .transform([this, operand](uint8_t r8) {
                auto next_value = r8 - 1;
                *set_r8(operand, r8 + 1);
                set_flag_z(next_value == 0);
                set_flag_n(true);
                set_flag_h((r8 & 0xF) == 0);
            });
    }

    std::expected<void, GameBoyError> GameBoy::ld_r8_imm8(uint8_t opcode) { 
        uint8_t dest = (opcode >> 4) & 0b11;
        
        return mmu.load_byte(pc)
            .and_then([this, dest](uint8_t imm8) {
                return set_r8(dest, imm8);
            });
    }

    void GameBoy::rlca(uint8_t opcode) { 
        auto a_value = get_a();
        set_a(a_value << 1);
        set_flag_c((a_value & 0x80) != 0);
        set_flag_z(false);
        set_flag_n(false);
        set_flag_h(false);
    }

    void GameBoy::rrca(uint8_t opcode) {
        auto a_value = get_a();
        set_a(a_value >> 1);
        set_flag_c((a_value & 1) != 0);
        set_flag_z(false);
        set_flag_n(false);
        set_flag_h(false);
    }

    void GameBoy::rla(uint8_t opcode) { 
        auto a_value = get_a();
        auto c_value = get_flag_c();
        set_a((a_value << 1) | c_value);
        set_flag_c((a_value & 0x80) != 0);
        set_flag_z(false);
        set_flag_n(false);
        set_flag_h(false);
    }

    void GameBoy::rra(uint8_t opcode) { 
        auto a_value = get_a();
        auto c_value = get_flag_c();
        set_a((a_value >> 1) | (static_cast<uint8_t>(c_value) << 7));
        set_flag_c((a_value & 1) != 0);
        set_flag_z(false);
        set_flag_n(false);
        set_flag_h(false);
    }

    void GameBoy::daa(uint8_t opcode) { 
        uint8_t adjustment = 0;
        auto a_value = get_a();

        if (get_flag_n()) {
            if (get_flag_h()) {
                adjustment += 6;
            }

            if (get_flag_c()) {
                adjustment += 0x60;
            }

            a_value -= adjustment;
        } else {
            if (get_flag_h() || (a_value & 0xf) > 9) {
                adjustment += 6;
            }

            if (get_flag_c() || a_value > 0x99) {
                adjustment += 0x60;
                set_flag_c(true);
            }

            a_value += adjustment;
        }

        set_flag_z(a_value == 0);
        set_flag_h(false);
    }

    void GameBoy::cpl(uint8_t opcode) { 
        set_a(~get_a());
        set_flag_n(true);
        set_flag_h(true);
    }

    void GameBoy::scf(uint8_t opcode) {
        set_flag_n(false);
        set_flag_h(false);
        set_flag_c(true);
    }

    void GameBoy::ccf(uint8_t opcode) {
        set_flag_n(false);
        set_flag_h(false);
        set_flag_c(!get_flag_c());
    }

    std::expected<void, GameBoyError> GameBoy::jr_imm8(uint8_t opcode) { 
        return mmu.load_byte(pc)
            .transform([this](uint8_t imm8) {
                pc += 1 + static_cast<int8_t>(imm8);
            });
    }

    std::expected<void, GameBoyError> GameBoy::jr_cond_imm8(uint8_t opcode) { 
        auto cond_code = (opcode >> 3) & 0b11;

        auto cond = get_cond(cond_code);

        if (!cond) {
            return std::unexpected(cond.error());
        }

        if (*cond) {
            return mmu.load_byte(pc)
                .transform([this](uint8_t imm8) {
                    pc += 1 + static_cast<int8_t>(imm8);
                });
        }

        return {};
    }

    // TODO: implement stop
    std::expected<void, GameBoyError> GameBoy::stop(uint8_t opcode) { 
        return std::unexpected(GameBoyError::unimplemented);
    }

    std::expected<void, GameBoyError> GameBoy::ld_r8_r8(uint8_t opcode) { 
        auto source = opcode & 0b111;
        auto dest = (opcode >> 3) & 0b111;

        return get_r8(source)
            .and_then([this, dest](uint8_t r8) {
                return set_r8(dest, r8);
            });
    }

    // TODO: implement halt
    std::expected<void, GameBoyError> GameBoy::halt(uint8_t opcode) { 
        return std::unexpected(GameBoyError::unimplemented);
    }

    std::expected<void, GameBoyError> GameBoy::add_a_r8(uint8_t opcode) { 
        auto operand = opcode & 0b111; 

        return get_r8(operand)
            .transform([this](uint8_t r8) {
                auto a_value = get_a();
                auto sum = a_value + r8;
                set_a(sum);
                set_flag_z(sum == 0);
                set_flag_n(false);
                set_flag_h((a_value & 0xf) > (sum & 0xf));
                set_flag_c(a_value > sum);
            });
    }

    std::expected<void, GameBoyError> GameBoy::adc_a_r8(uint8_t opcode) { 
        auto operand = opcode & 0b111; 

        return get_r8(operand)
            .transform([this](uint8_t r8) {
                auto a_value = get_a();
                auto sum = a_value + r8 + get_flag_c();
                set_a(sum);
                set_flag_z(sum == 0);
                set_flag_n(false);
                set_flag_h((a_value & 0xf) > (sum & 0xf));
                set_flag_c(a_value > sum);
            });
    }

    std::expected<void, GameBoyError> GameBoy::sub_a_r8(uint8_t opcode) { 
        auto operand = opcode & 0b111; 

        return get_r8(operand)
            .transform([this](uint8_t r8) {
                auto a_value = get_a();
                auto sum = a_value - r8;
                set_a(sum);
                set_flag_z(sum == 0);
                set_flag_n(true);
                set_flag_h((r8 & 0xf) > (a_value & 0xf));
                set_flag_c(r8 > a_value);
            });
    }

    std::expected<void, GameBoyError> GameBoy::sbc_a_r8(uint8_t opcode) { 
        auto operand = opcode & 0b111; 

        return get_r8(operand)
            .transform([this](uint8_t r8) {
                auto a_value = get_a();
                auto sum = a_value - (r8 + 1);
                set_a(sum);
                set_flag_z(sum == 0);
                set_flag_n(false);
                set_flag_h(((r8 + 1) & 0xf) > (a_value & 0xf));
                set_flag_c((r8 + 1) > a_value);
            });
    }

    std::expected<void, GameBoyError> GameBoy::and_a_r8(uint8_t opcode) { 
        auto operand = opcode & 0b111; 

        return get_r8(operand)
            .transform([this](uint8_t r8) {
                auto result = get_a() & r8;
                set_a(result);
                set_flag_z(result == 0);
                set_flag_n(false);
                set_flag_h(true);
                set_flag_c(false);
            });
    }

    std::expected<void, GameBoyError> GameBoy::xor_a_r8(uint8_t opcode) { 
        auto operand = opcode & 0b111; 

        return get_r8(operand)
            .transform([this](uint8_t r8) {
                auto result = get_a() ^ r8;
                set_a(result);
                set_flag_z(result == 0);
                set_flag_n(false);
                set_flag_h(false);
                set_flag_c(false);
            });
    }

    std::expected<void, GameBoyError> GameBoy::or_a_r8(uint8_t opcode) { 
        auto operand = opcode & 0b111; 

        return get_r8(operand)
            .transform([this](uint8_t r8) {
                auto result = get_a() | r8;
                set_a(result);
                set_flag_z(result == 0);
                set_flag_n(false);
                set_flag_h(false);
                set_flag_c(false);
            });
    }

    std::expected<void, GameBoyError> GameBoy::cp_a_r8(uint8_t opcode) { 
        auto operand = opcode & 0b111; 

        return get_r8(operand)
            .transform([this](uint8_t r8) {
                auto a_value = get_a();
                auto sum = a_value - r8;
                set_flag_z(sum == 0);
                set_flag_n(true);
                set_flag_h((r8 & 0xf) > (a_value & 0xf));
                set_flag_c(r8 > a_value);
            });
    }

    std::expected<void, GameBoyError> GameBoy::add_a_imm8(uint8_t opcode) { 
        return mmu.load_byte(pc)
            .transform([this](uint8_t imm8) {
                auto a_value = get_a();
                auto sum = a_value + imm8;
                set_a(sum);
                set_flag_z(sum == 0);
                set_flag_n(false);
                set_flag_h((a_value & 0xf) > (sum & 0xf));
                set_flag_c(a_value > sum);
                pc++;
            });
    }

    std::expected<void, GameBoyError> GameBoy::adc_a_imm8(uint8_t opcode) { 
        return mmu.load_byte(pc)
            .transform([this](uint8_t imm8) {
                auto a_value = get_a();
                auto sum = a_value + imm8 + get_flag_c();
                set_a(sum);
                set_flag_z(sum == 0);
                set_flag_n(false);
                set_flag_h((a_value & 0xf) > (sum & 0xf));
                set_flag_c(a_value > sum);
                pc++;
            });
    }

    std::expected<void, GameBoyError> GameBoy::sub_a_imm8(uint8_t opcode) { 
        return mmu.load_byte(pc)
            .transform([this](uint8_t imm8) {
                auto a_value = get_a();
                auto sum = a_value - imm8;
                set_a(sum);
                set_flag_z(sum == 0);
                set_flag_n(true);
                set_flag_h((imm8 & 0xf) > (a_value & 0xf));
                set_flag_c(imm8 > a_value);
                pc++;
            });
    }

    std::expected<void, GameBoyError> GameBoy::sbc_a_imm8(uint8_t opcode) { 
        return mmu.load_byte(pc)
            .transform([this](uint8_t imm8) {
                auto a_value = get_a();
                auto sum = a_value - (imm8 + 1);
                set_a(sum);
                set_flag_z(sum == 0);
                set_flag_n(false);
                set_flag_h(((imm8 + 1) & 0xf) > (a_value & 0xf));
                set_flag_c((imm8 + 1) > a_value);
                pc++;
            });
    }

    std::expected<void, GameBoyError> GameBoy::and_a_imm8(uint8_t opcode) { 
        return mmu.load_byte(pc)
            .transform([this](uint8_t imm8) {
                auto result = get_a() & imm8;
                set_a(result);
                set_flag_z(result == 0);
                set_flag_n(false);
                set_flag_h(true);
                set_flag_c(false);
                pc++;
            });
    }

    std::expected<void, GameBoyError> GameBoy::xor_a_imm8(uint8_t opcode) { 
        return mmu.load_byte(pc)
            .transform([this](uint8_t imm8) {
                auto result = get_a() ^ imm8;
                set_a(result);
                set_flag_z(result == 0);
                set_flag_n(false);
                set_flag_h(false);
                set_flag_c(false);
                pc++;
            });
    }

    std::expected<void, GameBoyError> GameBoy::or_a_imm8(uint8_t opcode) { 
        return mmu.load_byte(pc)
            .transform([this](uint8_t imm8) {
                auto result = get_a() | imm8;
                set_a(result);
                set_flag_z(result == 0);
                set_flag_n(false);
                set_flag_h(false);
                set_flag_c(false);
                pc++;
            });
    }

    std::expected<void, GameBoyError> GameBoy::cp_a_imm8(uint8_t opcode) { 
        return mmu.load_byte(pc)
            .transform([this](uint8_t imm8) {
                auto a_value = get_a();
                auto sum = a_value - imm8;
                set_flag_z(sum == 0);
                set_flag_n(true);
                set_flag_h((imm8 & 0xf) > (a_value & 0xf));
                set_flag_c(imm8 > a_value);
                pc++;
            });
    }

    std::expected<void, GameBoyError> GameBoy::ret_cond(uint8_t opcode) { 
        auto cond_code = (opcode >> 3) & 0b11;

        auto cond = get_cond(cond_code);

        if (!cond) {
            return std::unexpected(cond.error());
        }

        if (*cond) {
            return mmu.load_word(sp)
                .transform([this](uint8_t stk) {
                    pc = stk;
                    sp += 2;
                });
        }

        return {};
    }

    std::expected<void, GameBoyError> GameBoy::ret(uint8_t opcode) { 
        return mmu.load_word(sp)
            .transform([this](uint8_t stk) {
                pc = stk;
                sp += 2;
            });
    }
    
    // TODO: implement interrupts
    std::expected<void, GameBoyError> GameBoy::reti(uint8_t opcode) { 
        return std::unexpected(GameBoyError::unimplemented);
    }

    std::expected<void, GameBoyError> GameBoy::jp_cond_imm16(uint8_t opcode) { 
        auto cond_code = (opcode >> 3) & 0b11;

        auto cond = get_cond(cond_code);

        if (!cond) {
            return std::unexpected(cond.error());
        }

        if (*cond) {
            return mmu.load_word(pc)
                .transform([this](uint16_t imm16) {
                    pc = imm16;
                });
        }

        return {};
    }

    std::expected<void, GameBoyError> GameBoy::jp_imm16(uint8_t opcode) { 
        return mmu.load_word(pc)
            .transform([this](uint16_t imm16) {
                pc = imm16;
            });
    }

    void GameBoy::jp_hl(uint8_t opcode) { 
        pc = hl.pair;
    }

    std::expected<void, GameBoyError> GameBoy::call_cond_imm16(
        uint8_t opcode
    ) { 
        auto cond_code = (opcode >> 3) & 0b11;

        auto cond = get_cond(cond_code);

        if (!cond) {
            return std::unexpected(cond.error());
        }

        if (*cond) {
            return mmu.load_word(pc)
                .and_then([this](uint16_t imm16) {
                    return mmu.write_word(sp - 2, pc + 2)
                        .transform([this, imm16]() {
                            sp -= 2;
                            pc = imm16;
                        });
                });
        }

        return {};
    }

    std::expected<void, GameBoyError> GameBoy::call_imm16(uint8_t opcode) { 
        return mmu.load_word(pc)
            .and_then([this](uint16_t imm16) {
                return mmu.write_word(sp - 2, pc + 2)
                    .transform([this, imm16]() {
                        sp -= 2;
                        pc = imm16;
                    });
            });
    }

    // TODO: implement
    std::expected<void, GameBoyError> GameBoy::rst_tgt3(uint8_t opcode) { 
        return std::unexpected(GameBoyError::unimplemented);
    }

    std::expected<void, GameBoyError> GameBoy::pop_r16stk(uint8_t opcode) { 
        auto reg = (opcode >> 4) & 0b11;  

        return mmu.load_word(sp)
            .and_then([this, reg](uint16_t stk) {
                return set_r16stk(reg, stk);
            })
            .transform([this]() {
                sp += 2;
            });
    }

    std::expected<void, GameBoyError> GameBoy::push_r16stk(uint8_t opcode) { 
        auto reg = (opcode >> 4) & 0b11;  

        return get_r16stk(reg)
            .and_then([this](uint16_t r16stk) {
                return mmu.write_word(sp - 2, r16stk);
            })
            .transform([this]() {
                sp -= 2;
            });
    }

    std::expected<void, GameBoyError> GameBoy::ldh_c_a(uint8_t opcode) { 
        return mmu.write_byte(0xff00 + get_c(), get_a());
    }

    std::expected<void, GameBoyError> GameBoy::ldh_imm8_a(uint8_t opcode) { 
        return mmu.load_byte(pc)
            .and_then([this](uint8_t imm8) {
                return mmu.write_byte(0xff00 | imm8, get_a());
            })
            .transform([this]() {
                ++pc;
            });
    }

    std::expected<void, GameBoyError> GameBoy::ld_imm16_a(uint8_t opcode) { 
        return mmu.load_word(pc)
            .and_then([this](uint16_t imm16) {
                return mmu.write_byte(imm16, get_a());
            })
            .transform([this]() {
                pc += 2;
            });
    }

    std::expected<void, GameBoyError> GameBoy::ldh_a_c(uint8_t opcode) { 
        return mmu.load_byte(0xff00 + get_c())
            .transform([this](uint8_t value) {
                set_a(value);
            });
    }

    std::expected<void, GameBoyError> GameBoy::ldh_a_imm8(uint8_t opcode) { 
        return mmu.load_byte(pc)
            .and_then([this](uint8_t imm8) {
                return mmu.load_byte(0xff00 + imm8);
            })
            .transform([this](uint8_t value) {
                set_a(value);
                pc++;
            });
    }

    std::expected<void, GameBoyError> GameBoy::ld_a_imm16(uint8_t opcode) { 
        return mmu.load_word(pc)
            .and_then([this](uint8_t imm16) {
                return mmu.load_byte(imm16);
            })
            .transform([this](uint8_t value) {
                set_a(value);
                pc += 2;
            });
    }

    std::expected<void, GameBoyError> GameBoy::add_sp_imm8(uint8_t opcode) { 
        return mmu.load_byte(pc)
            .transform([this](uint8_t imm8) {
                auto sp_value = sp;
                sp += static_cast<int8_t>(imm8);
                set_flag_z(false);
                set_flag_n(false);
                set_flag_h((sp_value & 0xf) > (sp & 0xf));
                set_flag_c(sp_value > sp);
                ++pc;
            });
    }

    std::expected<void, GameBoyError> GameBoy::ld_hl_sp_imm8(uint8_t opcode) { 
        return mmu.load_byte(pc)
            .transform([this](uint8_t imm8) {
                auto sp_value = sp;
                auto sum = sp + static_cast<int8_t>(imm8);
                hl.pair = sum;
                set_flag_z(false);
                set_flag_n(false);
                set_flag_h((sp_value & 0xf) > (sum & 0xf));
                set_flag_c(sp_value > sum);
                ++pc;
            });
    }

    void GameBoy::ld_sp_hl(uint8_t opcode) { 
        sp = hl.pair;
    }

    // TODO: implement di
    std::expected<void, GameBoyError> GameBoy::di(uint8_t opcode) { 
        return std::unexpected(GameBoyError::unimplemented); 
    }

    // TODO: implement ei
    std::expected<void, GameBoyError> GameBoy::ei(uint8_t opcode) { 
        return std::unexpected(GameBoyError::unimplemented);
    }

    std::expected<void, GameBoyError> GameBoy::rlc_r8(uint8_t opcode) { 
        auto operand = opcode & 0b111;

        return get_r8(operand)
            .transform([this, operand](uint8_t r8) {
                auto value = get_a();
                auto shift = value << 1;
                *set_r8(operand, shift);
                set_flag_c((value & 0x80) != 0);
                set_flag_z(shift == 0);
                set_flag_n(false);
                set_flag_h(false);
            });
    }

    std::expected<void, GameBoyError> GameBoy::rrc_r8(uint8_t opcode) { 
        auto operand = opcode & 0b111;

        return get_r8(operand)
            .transform([this, operand](uint8_t r8) {
                auto shift = r8 >> 1;
                *set_r8(operand, shift);
                set_flag_c((r8 & 0x80) != 0);
                set_flag_z(shift == 0);
                set_flag_n(false);
                set_flag_h(false);
            });
    }

    std::expected<void, GameBoyError> GameBoy::rl_r8(uint8_t opcode) { 
        auto operand = opcode & 0b111;

        return get_r8(operand)
            .transform([this, operand](uint8_t r8) {
                auto c_value = get_flag_c();
                set_flag_c(r8 >> 7);
                auto shift = (r8 << 1) | c_value;
                *set_r8(operand, shift);
                set_flag_z(shift == 0);
                set_flag_n(false);
                set_flag_h(false);
            });
    }

    std::expected<void, GameBoyError> GameBoy::rr_r8(uint8_t opcode) { 
        auto operand = opcode & 0b111;

        return get_r8(operand)
            .transform([this, operand](uint8_t r8) {
                auto c_value = get_flag_c();
                set_flag_c(r8 & 1);
                auto shift = (r8 >> 1) | (c_value << 7);
                *set_r8(operand, shift);
                set_flag_z(shift == 0);
                set_flag_n(false);
                set_flag_h(false);
            });
    }

    std::expected<void, GameBoyError> GameBoy::sla_r8(uint8_t opcode) { 
        auto operand = opcode & 0b111; 

        return get_r8(operand)
            .transform([this, operand](uint8_t r8) {
                auto shift = r8 << 1;
                *set_r8(operand, shift);
                set_flag_z(shift == 0);
                set_flag_n(false);
                set_flag_h(false);
                set_flag_c(r8 >> 7);
            });
    }

    std::expected<void, GameBoyError> GameBoy::sra_r8(uint8_t opcode) { 
        auto operand = opcode & 0b111; 

        return get_r8(operand)
            .transform([this, operand](uint8_t r8) {
                auto shift = (r8 >> 1) | r8 & 0x80;
                *set_r8(operand, shift);
                set_flag_z(shift == 0);
                set_flag_n(false);
                set_flag_h(false);
                set_flag_c(r8 & 1);
            });
    }

    std::expected<void, GameBoyError> GameBoy::swap_r8(uint8_t opcode) { 
        auto operand = opcode & 0b111;

        return get_r8(operand)
            .transform([this, operand](uint8_t r8) {
                auto swap = ((r8 & 0xf) << 4) | ((r8 & 0xf0) >> 4);
                *set_r8(operand, swap);
                set_flag_z(swap == 0);
                set_flag_n(false);
                set_flag_h(false);
                set_flag_c(false);
            });
    }

    std::expected<void, GameBoyError> GameBoy::srl_r8(uint8_t opcode) { 
        auto operand = opcode & 0b111; 

        return get_r8(operand)
            .transform([this, operand](uint8_t r8) {
                auto shift = r8 >> 1;
                *set_r8(operand, shift);
                set_flag_z(shift == 0);
                set_flag_n(false);
                set_flag_h(false);
                set_flag_c(r8 & 1);
            });
    }

    std::expected<void, GameBoyError> GameBoy::bit_b3_r8(uint8_t opcode) { 
        auto operand = opcode & 0b111;
        auto bit3 = (opcode >> 3) & 0b111;

        return get_r8(operand)
            .transform([this, bit3](uint8_t r8) {
                set_flag_z(((r8 >> bit3) & 1) == 0); 
                set_flag_n(false);
                set_flag_h(true);
            });
    }

    std::expected<void, GameBoyError> GameBoy::res_b3_r8(uint8_t opcode) { 
        auto operand = opcode & 0b111;
        auto bit3 = (opcode >> 3) & 0b111;

        return get_r8(operand)
            .transform([this, operand, bit3](uint8_t r8) {
                *set_r8(operand, r8 & ~(1 << bit3));
            });
    }

    std::expected<void, GameBoyError> GameBoy::set_b3_r8(uint8_t opcode) { 
        auto operand = opcode & 0b111;
        auto bit3 = (opcode >> 3) & 0b111;

        return get_r8(operand)
            .transform([this, operand, bit3](uint8_t r8) {
                *set_r8(operand, r8 & (1 << bit3));
            });
    }
}
