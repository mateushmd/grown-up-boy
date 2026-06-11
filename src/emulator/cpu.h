#pragma once

#include <cstdint>
#include <expected>
#include "defs.h"
#include "bus.h"
#include "register.h"

namespace emulator {
    class CPU {
        private:
            union RegisterPair af;
            union RegisterPair bc;
            union RegisterPair de;
            union RegisterPair hl;
            uint16_t sp;
            uint16_t pc;

            bool ime;

            Bus bus;

            bool cb_flag;

            inline uint8_t get_a(void);
            inline uint8_t get_f(void);
            inline uint8_t get_b(void);
            inline uint8_t get_c(void);
            inline uint8_t get_d(void);
            inline uint8_t get_e(void);
            inline uint8_t get_h(void);
            inline uint8_t get_l(void);

            inline void set_a(uint8_t value);
            inline void set_f(uint8_t value);
            inline void set_b(uint8_t value);
            inline void set_c(uint8_t value);
            inline void set_d(uint8_t value);
            inline void set_e(uint8_t value);
            inline void set_h(uint8_t value);
            inline void set_l(uint8_t value);

            inline bool get_flag_z(void);
            inline bool get_flag_n(void);
            inline bool get_flag_h(void);
            inline bool get_flag_c(void);

            void set_flag_z(bool value);
            void set_flag_n(bool value);
            void set_flag_h(bool value);
            void set_flag_c(bool value);

            std::expected<uint8_t, GameBoyError> get_r8(uint8_t r8);
            std::expected<void, GameBoyError> set_r8(
                uint8_t r8, uint8_t value
            );

            std::expected<uint16_t, GameBoyError> get_r16(uint8_t r16);
            std::expected<void, GameBoyError> set_r16(
                uint8_t r16, uint16_t value
            );
            
            std::expected<uint16_t, GameBoyError> get_r16stk(uint8_t r16stk);
            std::expected<void, GameBoyError> set_r16stk(
                uint8_t r16stk, uint16_t value
            );
            
            std::expected<uint16_t, GameBoyError> get_r16mem(uint8_t r16mem);
            std::expected<void, GameBoyError> set_r16mem(
                uint8_t r16mem, uint16_t value
            );

            std::expected<bool, GameBoyError> get_cond(uint8_t cond);

            std::expected<uint16_t, GameBoyError> load_word(uint16_t address);
            std::expected<void, GameBoyError> store_word(
                uint16_t address, uint16_t value
            );

            std::expected<void, GameBoyError> ld_r16_imm16(uint8_t opcode);
            std::expected<void, GameBoyError> ld_r16mem_a(uint8_t opcode);
            std::expected<void, GameBoyError> ld_a_r16mem(uint8_t opcode);
            std::expected<void, GameBoyError> ld_imm16_sp(uint8_t opcode);
            std::expected<void, GameBoyError> inc_r16(uint8_t opcode);
            std::expected<void, GameBoyError> dec_r16(uint8_t opcode);
            std::expected<void, GameBoyError> add_hl_r16(uint8_t opcode);
            std::expected<void, GameBoyError> inc_r8(uint8_t opcode);
            std::expected<void, GameBoyError> dec_r8(uint8_t opcode);
            std::expected<void, GameBoyError> ld_r8_imm8(uint8_t opcode);
            std::expected<void, GameBoyError> rlca(uint8_t opcode);
            std::expected<void, GameBoyError> rrca(uint8_t opcode);
            std::expected<void, GameBoyError> rla(uint8_t opcode);
            std::expected<void, GameBoyError> rra(uint8_t opcode);
            std::expected<void, GameBoyError> daa(uint8_t opcode);
            std::expected<void, GameBoyError> cpl(uint8_t opcode);
            std::expected<void, GameBoyError> scf(uint8_t opcode);
            std::expected<void, GameBoyError> ccf(uint8_t opcode);
            std::expected<void, GameBoyError> jr_imm8(uint8_t opcode);
            std::expected<void, GameBoyError> jr_cond_imm8(uint8_t opcode);
            std::expected<void, GameBoyError> stop(uint8_t opcode);
            std::expected<void, GameBoyError> ld_r8_r8(uint8_t opcode);
            std::expected<void, GameBoyError> halt(uint8_t opcode);
            std::expected<void, GameBoyError> add_a_r8(uint8_t opcode);
            std::expected<void, GameBoyError> adc_a_r8(uint8_t opcode);
            std::expected<void, GameBoyError> sub_a_r8(uint8_t opcode);
            std::expected<void, GameBoyError> sbc_a_r8(uint8_t opcode);
            std::expected<void, GameBoyError> and_a_r8(uint8_t opcode);
            std::expected<void, GameBoyError> xor_a_r8(uint8_t opcode);
            std::expected<void, GameBoyError> or_a_r8(uint8_t opcode);
            std::expected<void, GameBoyError> cp_a_r8(uint8_t opcode);
            std::expected<void, GameBoyError> add_a_imm8(uint8_t opcode);
            std::expected<void, GameBoyError> adc_a_imm8(uint8_t opcode);
            std::expected<void, GameBoyError> sub_a_imm8(uint8_t opcode);
            std::expected<void, GameBoyError> sbc_a_imm8(uint8_t opcode);
            std::expected<void, GameBoyError> and_a_imm8(uint8_t opcode);
            std::expected<void, GameBoyError> xor_a_imm8(uint8_t opcode);
            std::expected<void, GameBoyError> or_a_imm8(uint8_t opcode);
            std::expected<void, GameBoyError> cp_a_imm8(uint8_t opcode);
            std::expected<void, GameBoyError> ret_cond(uint8_t opcode);
            std::expected<void, GameBoyError> ret(uint8_t opcode);
            std::expected<void, GameBoyError> reti(uint8_t opcode);
            std::expected<void, GameBoyError> jp_cond_imm16(uint8_t opcode);
            std::expected<void, GameBoyError> jp_imm16(uint8_t opcode);
            std::expected<void, GameBoyError> jp_hl(uint8_t opcode);
            std::expected<void, GameBoyError> call_cond_imm16(uint8_t opcode);
            std::expected<void, GameBoyError> call_imm16(uint8_t opcode);
            std::expected<void, GameBoyError> rst_tgt3(uint8_t opcode);
            std::expected<void, GameBoyError> pop_r16stk(uint8_t opcode);
            std::expected<void, GameBoyError> push_r16stk(uint8_t opcode);
            std::expected<void, GameBoyError> ldh_c_a(uint8_t opcode);
            std::expected<void, GameBoyError> ldh_imm8_a(uint8_t opcode);
            std::expected<void, GameBoyError> ld_imm16_a(uint8_t opcode);
            std::expected<void, GameBoyError> ldh_a_c(uint8_t opcode);
            std::expected<void, GameBoyError> ldh_a_imm8(uint8_t opcode);
            std::expected<void, GameBoyError> ld_a_imm16(uint8_t opcode);
            std::expected<void, GameBoyError> add_sp_imm8(uint8_t opcode);
            std::expected<void, GameBoyError> ld_hl_sp_imm8(uint8_t opcode);
            std::expected<void, GameBoyError> ld_sp_hl(uint8_t opcode);
            std::expected<void, GameBoyError> di(uint8_t opcode);
            std::expected<void, GameBoyError> ei(uint8_t opcode);
            std::expected<void, GameBoyError> rlc_r8(uint8_t opcode);
            std::expected<void, GameBoyError> rrc_r8(uint8_t opcode);
            std::expected<void, GameBoyError> rl_r8(uint8_t opcode);
            std::expected<void, GameBoyError> rr_r8(uint8_t opcode);
            std::expected<void, GameBoyError> sla_r8(uint8_t opcode);
            std::expected<void, GameBoyError> sra_r8(uint8_t opcode);
            std::expected<void, GameBoyError> swap_r8(uint8_t opcode);
            std::expected<void, GameBoyError> srl_r8(uint8_t opcode);
            std::expected<void, GameBoyError> bit_b3_r8(uint8_t opcode);
            std::expected<void, GameBoyError> res_b3_r8(uint8_t opcode);
            std::expected<void, GameBoyError> set_b3_r8(uint8_t opcode);

            std::expected<void, GameBoyError> decode_execute(uint8_t opcode);
            std::expected<void, GameBoyError> block0(uint8_t opcode);
            std::expected<void, GameBoyError> block1(uint8_t opcode);
            std::expected<void, GameBoyError> block2(uint8_t opcode);
            std::expected<void, GameBoyError> block3(uint8_t opcode);
            std::expected<void, GameBoyError> cb_prefix(uint8_t opcode);

        public:
    };
}
