#pragma once

#include <cstdint>

#include "cpu/register_pair.hpp"
#include "event.hpp"
#include "memory.hpp"

namespace gameboy
{
enum class States
{
    FETCH,
    EXECUTE,
    HALT,
    STOP
};

enum class R16Source
{
    DEFAULT,
    STACK,
    MEMORY
};

constexpr uint8_t CARRY = 0x4;
constexpr uint8_t HALF_CARRY = 0x5;
constexpr uint8_t SUBTRACTION = 0x6;
constexpr uint8_t ZERO = 0x7;

class CPU
{
  private:
    uint8_t A, F, B, C, D, E, H, L, IE;
    uint16_t PC, SP;
    cpu::RegisterPair BC, DE, HL, AF;

    bool IME;

    Memory &memory;

    bool cbFlag;
    States state;

    uint8_t fetched;

    bool branch;

    uint8_t getReg_8(const uint8_t);
    uint16_t getReg_16(const uint8_t, const R16Source);

    void setReg_8(const uint8_t, const uint8_t);
    void setReg_16(const uint8_t, const uint16_t, const R16Source);

    bool getFlag(const uint8_t);
    void setFlag(const uint8_t, const bool);
    bool getCondition(const uint8_t);

    uint8_t fetch();
    uint8_t execute(const uint8_t);
    void doExecute(const uint8_t);

    // Helpers
    uint16_t fetchWord();

    inline void clock();

    void rla(const bool);
    void rra(const bool);
    void daa();
    void cpl();
    void scf();
    void ccf();
    void jp_imm16();
    void jp_cond_imm16(const uint8_t);
    void jp_hl();
    void jr_imm8();
    void jr_cond_imm8(const uint8_t);
    void call_cond_imm16(const uint8_t);
    void call_imm16();
    void ret();
    void ret_cond(const uint8_t);
    void reti();
    void add_a_imm8(const bool);
    void add_a_r8(const uint8_t, const bool);
    void sub_a_imm8(const bool);
    void sub_a_r8(const uint8_t, const bool);
    void and_a_imm8();
    void and_a_r8(const uint8_t);
    void xor_a_imm8();
    void xor_a_r8(const uint8_t);
    void or_a_imm8();
    void or_a_r8(const uint8_t);
    void cp_a_imm8();
    void cp_a_r8(const uint8_t);
    void add_sp_imm8();
    void inc_r8(const uint8_t);
    void dec_r8(const uint8_t);
    void inc_r16(const uint8_t);
    void dec_r16(const uint8_t);
    void ld_r8_imm8(const uint8_t);
    void ld_r8_r8(const uint8_t);
    void add_hl_r16(const uint8_t);
    void push_r16(const uint8_t);
    void pop_r16(const uint8_t);
    void halt();
    void stop();
    void ei();
    void di();
    void rst_tgt3(const uint8_t);
    void ld_r16_imm16(const uint8_t);
    void ld_r16mem_a(const uint8_t);
    void ld_a_r16mem(const uint8_t);
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

    void rl_r8(const uint8_t, const bool);
    void rr_r8(const uint8_t, const bool);
    void sla_r8(const uint8_t);
    void sra_r8(const uint8_t);
    void swap_r8(const uint8_t);
    void srl_r8(const uint8_t);
    void bit_b3_r8(const uint8_t);
    void res_b3_r8(const uint8_t);
    void set_b3_r8(const uint8_t);

  public:
    util::Event<uint8_t, uint16_t> onFetch;
    util::Event<uint8_t, uint8_t, uint16_t> onExecute;
    util::Event<uint8_t> onUpdate;
    util::Event<> onHalt;

    CPU(Memory &, bool);

    uint8_t update();

    uint16_t getAF();
    uint16_t getBC();
    uint16_t getDE();
    uint16_t getHL();
    uint16_t getPC();
    uint16_t getSP();

    uint8_t getA();
    uint8_t getF();
    uint8_t getB();
    uint8_t getC();
    uint8_t getD();
    uint8_t getE();
    uint8_t getH();
    uint8_t getL();
};
} // namespace gameboy
