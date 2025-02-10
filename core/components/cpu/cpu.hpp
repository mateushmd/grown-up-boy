// cpu.hpp
#pragma once

#include <functional>
#include <array>
#include <bit>

#include "types.hpp"
#include "event.hpp"
#include "bus.hpp"

namespace emulator::components
{
    enum class States
    {
        FETCH,
        EXECUTE,
        HALT,
        STOP
    };

    enum class R16ReadSource
    {
        DEFAULT,
        STACK,
        MEMORY
    };

    struct Registers
    {
        union
        {
            struct 
            {
                byte A, F;
                byte B, C;
                byte D, E;
                byte H, L;
            };

            struct
            {
                word AF;
                word BC;
                word DE;
                word HL;
            };
        };

        word PC;
        word SP;
        byte IE;
    };

    class CPU
    {
        static constexpr byte CARRY = 0x4;
        static constexpr byte HALF_CARRY = 0x5;
        static constexpr byte SUBTRACTION = 0x6;
        static constexpr byte ZERO = 0x7;

    private:
        Registers registers;

        bool IME;

        Bus &bus;

        bool cbFlag;
        States state;

        byte fetched;

        word &AF();

        void boot();

        byte &getReg_8(const byte);
        word &getReg_16(const byte, const R16ReadSource);

        bool getFlag(const byte);
        void setFlag(const byte, const bool);
        bool getCondition(const byte);

        byte fetch();
        void execute(const byte);
        void doExecute(const byte);

        // Helpers
        word fetchWord();

#pragma region Instructions
        void rla(const bool);
        void rra(const bool);
        void daa();
        void cpl();
        void scf();
        void ccf();
        void jp_imm16();
        void jp_cond_imm16(const byte);
        void jp_hl();
        void jr_imm8();
        void jr_cond_imm8(const byte);
        void call_cond_imm16(const byte);
        void call_imm16();
        void ret();
        void ret_cond(const byte);
        void reti();
        void add_a_imm8(const bool);
        void add_a_r8(const byte, const bool);
        void sub_a_imm8(const bool);
        void sub_a_r8(const byte, const bool);
        void and_a_imm8();
        void and_a_r8(const byte);
        void xor_a_imm8();
        void xor_a_r8(const byte);
        void or_a_imm8();
        void or_a_r8(const byte);
        void cp_a_imm8();
        void cp_a_r8(const byte);
        void add_sp_imm8();
        void inc_r8(const byte);
        void dec_r8(const byte);
        void inc_r16(const byte);
        void dec_r16(const byte);
        void ld_r8_imm8(const byte);
        void ld_r8_r8(const byte);
        void add_hl_r16(const byte);
        void push_r16(const byte);
        void pop_r16(const byte);
        void halt();
        void stop();
        void ei();
        void di();
        void rst_tgt3(const byte);
        void ld_r16_imm16(const byte);
        void ld_r16mem_a(const byte);
        void ld_a_r16mem(const byte);
        void ld_imm16mem_sp();
        void ld_imm16mem_a();
        void ldh_imm8mem_a();
        void ldh_c_a();
        void ldh_a_imm8mem();
        void ldh_a_c();
        void ld_a_imm16mem();
        void ld_sp_imm16();
        void ld_hl_sp_p_e8();
        void ld_sp_hl();

        void rl_r8(const byte, const bool);
        void rr_r8(const byte, const bool);
        void sla_r8(const byte);
        void sra_r8(const byte);
        void swap_r8(const byte);
        void srl_r8(const byte);
        void bit_b3_r8(const byte);
        void res_b3_r8(const byte);
        void set_b3_r8(const byte);
#pragma endregion Instructions

    public:
        util::Event<byte, word> onFetch;
        util::Event<byte, word> onExecute;
        util::Event<> onHalt;

        CPU(Bus &);

        void clock();

        word getAF() { return registers.AF; }
        word getBC() { return registers.BC; }
        word getDE() { return registers.DE; }
        word getHL() { return registers.HL; }
        word getPC() { return registers.PC; }
        word getSP() { return registers.SP; }
    };
}