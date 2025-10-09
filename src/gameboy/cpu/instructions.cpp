#include "../bus.hpp"
#include "../cpu.hpp"

#include <cstdint>

namespace gameboy
{
void CPU::rla(const bool carry)
{
    uint8_t value = A;
    uint8_t msb = (value & 0x80) != 0 ? 1 : 0;
    const uint8_t carryValue = carry && getFlag(CARRY) ? 1 : 0;
    value <<= 1;
    value += carry ? carryValue : msb;
    A = value;

    setFlag(CARRY, msb);
    setFlag(HALF_CARRY, false);
    setFlag(SUBTRACTION, false);
    setFlag(ZERO, false);
}
void CPU::rra(const bool carry)
{
    uint8_t value = A;
    uint8_t lsb = (value & 0x1) != 0 ? 0x80 : 0;
    const uint8_t carryValue = carry && getFlag(CARRY) ? 0x80 : 0;
    value >>= 1;
    value += carry ? carryValue : lsb;
    A = value;

    setFlag(CARRY, lsb);
    setFlag(HALF_CARRY, false);
    setFlag(SUBTRACTION, false);
    setFlag(ZERO, false);
}
void CPU::jr_imm8()
{
    // TODO: decide what will happen in infinite loops (maybe a quality of life
    // setting)

    const int8_t offset = static_cast<int8_t>(fetch());

    PC = PC + offset;
}
void CPU::daa()
{
    uint8_t value = A;
    uint8_t adjust = 0;

    if (getFlag(SUBTRACTION))
    {
        if (getFlag(HALF_CARRY))
            adjust -= 0x06;
        if (getFlag(CARRY))
            adjust -= 0x60;
    }
    else
    {
        if (getFlag(HALF_CARRY) || (value & 0x0F) > 0x09)
            adjust += 0x06;
        if (getFlag(CARRY) || value > 0x99)
        {
            adjust += 0x60;
            setFlag(CARRY, true);
        }
    }

    value += adjust;
    A = value;

    setFlag(ZERO, value == 0);
    setFlag(HALF_CARRY, false);
}
void CPU::cpl()
{
    A = ~A;

    setFlag(HALF_CARRY, true);
    setFlag(SUBTRACTION, true);
}
void CPU::scf()
{
    setFlag(CARRY, true);
    setFlag(HALF_CARRY, false);
    setFlag(SUBTRACTION, false);
}
void CPU::ccf()
{
    setFlag(CARRY, !getFlag(CARRY));
    setFlag(HALF_CARRY, false);
    setFlag(SUBTRACTION, false);
}
void CPU::jp_imm16() { PC = fetchWord(); }
void CPU::add_a_imm8(const bool carry)
{
    const uint8_t value = fetch();
    uint8_t regValue = A;
    const uint8_t carryValue = carry && getFlag(CARRY) ? 1 : 0;
    const uint8_t sum = regValue + value + carryValue;

    setFlag(CARRY, (0xFF - regValue) < (value + carryValue));
    setFlag(HALF_CARRY, ((value & 0xF) + (regValue & 0xF)) > 0xF);
    setFlag(SUBTRACTION, false);
    setFlag(ZERO, sum == 0);

    A = sum;
}
void CPU::sub_a_imm8(const bool carry)
{
    const uint8_t value = fetch();
    const uint8_t carryValue = carry && getFlag(CARRY) ? 1 : 0;
    const uint8_t accValue = A;
    const uint8_t sub = accValue - value - carryValue;

    setFlag(CARRY, (value + carryValue) > accValue);
    setFlag(HALF_CARRY, ((value + carryValue) & 0xF) > (accValue & 0xF));
    setFlag(SUBTRACTION, true);
    setFlag(ZERO, sub == 0);

    A = sub;
}
void CPU::and_a_imm8()
{
    const uint8_t value = fetch();
    A = A & value;

    setFlag(CARRY, false);
    setFlag(HALF_CARRY, true);
    setFlag(SUBTRACTION, false);
    setFlag(ZERO, A == 0);
}
void CPU::jp_hl() { PC = HL.get(); }
void CPU::add_sp_imm8()
{
    const int8_t offset = static_cast<int8_t>(fetch());

    SP = SP + offset;
}
void CPU::xor_a_imm8()
{
    const uint8_t value = fetch();
    A = A ^ value;

    setFlag(CARRY, false);
    setFlag(HALF_CARRY, false);
    setFlag(SUBTRACTION, false);
}
void CPU::or_a_imm8()
{
    const uint8_t value = fetch();
    A = A | value;

    setFlag(CARRY, false);
    setFlag(HALF_CARRY, false);
    setFlag(SUBTRACTION, false);
    setFlag(ZERO, A == 0);
}
void CPU::cp_a_imm8()
{
    const uint8_t value = fetch();
    uint8_t regValue = A;
    const uint8_t sub = regValue - value;

    setFlag(CARRY, value > regValue);
    setFlag(HALF_CARRY, (value & 0xF) > (regValue & 0xF));
    setFlag(SUBTRACTION, true);
    setFlag(ZERO, sub == 0);
}
void CPU::call_imm16()
{
    const uint16_t address = fetchWord();

    const uint8_t low = static_cast<uint8_t>(PC);
    const uint8_t high = static_cast<uint8_t>(PC >> 8);

    SP--;
    bus.write(SP, high);
    SP--;
    bus.write(SP, low);

    PC = address;
}
void CPU::ret()
{
    const uint8_t low = bus.read(SP);
    SP++;
    const uint8_t high = bus.read(SP);
    SP++;

    const uint16_t address = (static_cast<uint16_t>(high) << 8) + low;

    PC = address;
}
void CPU::reti()
{
    IME = 1;

    const uint8_t low = bus.read(SP);
    SP++;
    const uint8_t high = bus.read(SP);
    SP++;

    const uint16_t address = (static_cast<uint16_t>(high) << 8) + low;

    PC = address;
}
void CPU::halt()
{
    // TODO: what halt does?
    state = States::HALT;
    // NOTE: temporary behaviour for testing
    onHalt.notify();
}
void CPU::stop() { state = States::STOP; }
void CPU::ei() { IME = 1; }
void CPU::di() { IME = 0; }
void CPU::inc_r16(const uint8_t opcode)
{
    const uint8_t encodedReg = (opcode & 0x30) >> 4;
    setReg_16(encodedReg, getReg_16(encodedReg, R16Source::DEFAULT) + 1,
              R16Source::DEFAULT);
}
void CPU::inc_r8(const uint8_t opcode)
{
    const uint8_t encodedReg = (opcode & 0x38) >> 3;
    uint8_t reg = getReg_8(encodedReg);

    setFlag(HALF_CARRY, ((reg & 0xF) + 1) > 0xF);
    setFlag(SUBTRACTION, false);
    setFlag(ZERO, (reg + 1) == 0);

    ++reg;
    setReg_8(encodedReg, reg);
}
void CPU::dec_r8(const uint8_t opcode)
{
    const uint8_t encodedReg = (opcode & 0x38) >> 3;
    uint8_t reg = getReg_8(encodedReg);

    setFlag(HALF_CARRY, (reg & 0xF) == 0);
    setFlag(SUBTRACTION, true);
    setFlag(ZERO, (reg - 1) == 0);

    --reg;
    setReg_8(encodedReg, reg);
}
void CPU::ld_r8_imm8(const uint8_t opcode)
{
    const uint8_t encodedReg = (opcode & 0x38) >> 3;
    const uint8_t immediateuint8_t = fetch();
    setReg_8(encodedReg, immediateuint8_t);
}
void CPU::dec_r16(const uint8_t opcode)
{
    const uint8_t encodedReg = (opcode & 0x30) >> 3;
    uint16_t reg = getReg_16(encodedReg, R16Source::DEFAULT);
    --reg;
    setReg_16(encodedReg, reg, R16Source::DEFAULT);
}
void CPU::ld_r8_r8(const uint8_t opcode)
{
    const uint8_t encodedDest = (opcode & 0x38) >> 3;
    const uint8_t encodedSrc = (opcode & 0x07);
    const uint8_t srcValue = getReg_8(encodedSrc);
    setReg_8(encodedDest, srcValue);
}
void CPU::add_a_r8(const uint8_t opcode, const bool carry)
{
    const uint8_t encodedReg = (opcode & 0x07);
    const uint8_t value = getReg_8(encodedReg);
    uint8_t aValue = A;

    const uint8_t carryValue = carry && getFlag(CARRY) ? 1 : 0;
    const uint8_t sum = aValue + value + carryValue;

    setFlag(CARRY, (0xFF - aValue) < (value + carryValue));
    setFlag(HALF_CARRY, ((value & 0xF) + (aValue & 0xF)) > 0xF);
    setFlag(SUBTRACTION, false);
    setFlag(ZERO, sum == 0);

    A = sum;
}
void CPU::sub_a_r8(const uint8_t opcode, const bool carry)
{
    const uint8_t encodedReg = (opcode & 0x07);
    const uint8_t value = getReg_8(encodedReg);
    uint8_t aValue = A;

    const uint8_t carryValue = carry && getFlag(CARRY) ? 1 : 0;
    const uint8_t sub = aValue - value - carryValue;

    setFlag(CARRY, (value + carryValue) > aValue);
    setFlag(HALF_CARRY, ((value + carryValue) & 0xF) > (aValue & 0xF));
    setFlag(SUBTRACTION, true);
    setFlag(ZERO, sub == 0);

    A = sub;
}
void CPU::and_a_r8(const uint8_t opcode)
{
    const uint8_t encodedReg = (opcode & 0x07);
    const uint8_t value = getReg_8(encodedReg);

    A = A & value;

    setFlag(CARRY, false);
    setFlag(HALF_CARRY, true);
    setFlag(SUBTRACTION, false);
    setFlag(ZERO, A == 0);
}
void CPU::xor_a_r8(const uint8_t opcode)
{
    const uint8_t encodedReg = (opcode & 0x07);
    const uint8_t value = getReg_8(encodedReg);

    A = A ^ value;

    setFlag(CARRY, false);
    setFlag(HALF_CARRY, false);
    setFlag(SUBTRACTION, false);
    setFlag(ZERO, A == 0);
}
void CPU::or_a_r8(const uint8_t opcode)
{
    const uint8_t encodedReg = (opcode & 0x07);
    const uint8_t value = getReg_8(encodedReg);

    A = A | value;

    setFlag(CARRY, false);
    setFlag(HALF_CARRY, false);
    setFlag(SUBTRACTION, false);
    setFlag(ZERO, A == 0);
}
void CPU::cp_a_r8(const uint8_t opcode)
{
    const uint8_t encodedReg = (opcode & 0x07);
    const uint8_t value = getReg_8(encodedReg);
    uint8_t aValue = A;
    const uint8_t sub = aValue - value;

    setFlag(CARRY, value > aValue);
    setFlag(HALF_CARRY, (value & 0xF) > (aValue & 0xF));
    setFlag(SUBTRACTION, true);
    setFlag(ZERO, sub == 0);
}
void CPU::jp_cond_imm16(const uint8_t opcode)
{
    const uint8_t encodedCondition = (opcode & 0x18) >> 3;
    const uint16_t value = fetchWord();

    if (getCondition(encodedCondition))
    {
        PC = value;
        branch = true;
    }
}
void CPU::jr_cond_imm8(const uint8_t opcode)
{
    const uint8_t encodedCondition = (opcode & 0x18) >> 3;
    const int8_t offset = static_cast<int8_t>(fetch());

    if (getCondition(encodedCondition))
    {
        PC = PC + offset;
        branch = true;
    }
}
void CPU::call_cond_imm16(const uint8_t opcode)
{
    const uint16_t address = fetchWord();

    const uint8_t encodedCondition = (opcode & 0x18) >> 3;

    if (!getCondition(encodedCondition))
        return;

    branch = true;

    const uint8_t low = static_cast<uint8_t>(PC);
    const uint8_t high = static_cast<uint8_t>(PC >> 8);

    SP--;
    bus.write(SP, high);
    SP--;
    bus.write(SP, low);

    PC = address;
}
void CPU::ret_cond(const uint8_t opcode)
{
    const uint8_t encodedCondition = (opcode & 0x18) >> 3;

    if (!getCondition(encodedCondition))
        return;

    branch = true;

    const uint8_t low = bus.read(SP);
    SP++;
    const uint8_t high = bus.read(SP);
    SP++;

    const uint16_t address = (static_cast<uint16_t>(high) << 8) + low;

    PC = address;
}
void CPU::add_hl_r16(const uint8_t opcode)
{
    const uint8_t encoded = (opcode & 0x30) >> 4;
    const uint16_t reg = getReg_16(encoded, R16Source::DEFAULT);
    uint16_t hlValue = HL.get();
    const uint16_t sum = reg + hlValue;

    setFlag(CARRY, reg > (0xFFFF - hlValue));
    setFlag(HALF_CARRY, ((hlValue & 0xFFF) + (reg & 0xFFF)) > 0xFFF);
    setFlag(SUBTRACTION, false);

    HL.set(sum);
}
void CPU::push_r16(const uint8_t opcode)
{
    const uint8_t encoded = (opcode & 0x30) >> 4;
    const uint16_t reg = getReg_16(encoded, R16Source::STACK);
    const uint8_t low = static_cast<uint8_t>(reg);
    const uint8_t high = static_cast<uint8_t>(reg >> 8);

    SP--;
    bus.write(SP, high);
    SP--;
    bus.write(SP, low);
}
void CPU::pop_r16(const uint8_t opcode)
{
    const uint8_t encoded = (opcode & 0x30) >> 4;

    const uint8_t low = bus.read(SP);
    SP++;
    const uint8_t high = bus.read(SP);
    SP++;

    const uint16_t value = (static_cast<uint16_t>(high) << 8) | low;

    setReg_16(encoded, value, R16Source::STACK);
}
void CPU::rst_tgt3(const uint8_t opcode) { PC = opcode & 0x38; }
void CPU::ld_r16_imm16(const uint8_t opcode)
{
    const uint8_t encodedReg = (opcode & 0x30) >> 4;
    setReg_16(encodedReg, fetchWord(), R16Source::DEFAULT);
}
void CPU::ld_r16mem_a(const uint8_t opcode)
{
    const uint8_t encodedReg = (opcode & 0x30) >> 4;
    const uint16_t reg = getReg_16(encodedReg, R16Source::MEMORY);
    bus.write(reg, A);
}
void CPU::ld_a_r16mem(const uint8_t opcode)
{
    const uint8_t encodedReg = (opcode & 0x30) >> 4;
    const uint16_t reg = getReg_16(encodedReg, R16Source::MEMORY);
    A = bus.read(reg);
}
void CPU::ld_imm16mem_sp()
{
    const uint16_t address = fetchWord();
    SP = bus.read(address);
}
void CPU::ld_imm16mem_a()
{
    const uint16_t address = fetchWord();
    bus.write(address, A);
}
void CPU::ldh_imm8mem_a()
{
    const uint8_t address = fetch();
    bus.write(0xFF00 + address, A);
}
void CPU::ldh_c_a() { bus.write(0xFF00 + C, A); }
void CPU::ldh_a_imm8mem()
{
    const uint8_t address = fetch();
    A = bus.read(0xFF00 + address);
}
void CPU::ldh_a_c() { A = bus.read(0xFF00 + C); }
void CPU::ld_a_imm16mem()
{
    const uint16_t address = fetchWord();
    A = bus.read(address);
}
void CPU::ld_sp_imm16()
{
    const uint16_t value = fetchWord();
    SP = value;
}
void CPU::ld_hl_sp_p_e8()
{
    const int8_t offset = static_cast<int8_t>(fetch());
    HL.set(SP + offset);
}
void CPU::ld_sp_hl() { SP = HL.get(); }

void CPU::rl_r8(const uint8_t opcode, const bool carry)
{
    const uint8_t encodedReg = (opcode & 0x07);
    uint8_t reg = getReg_8(encodedReg);

    uint8_t msb = (reg & 0x80) != 0 ? 1 : 0;
    const uint8_t carryValue = carry && getFlag(CARRY) ? 1 : 0;
    reg <<= 1;
    reg += carry ? carryValue : msb;
    setReg_8(encodedReg, reg);

    setFlag(CARRY, msb != 0);
    setFlag(HALF_CARRY, false);
    setFlag(SUBTRACTION, false);
    setFlag(ZERO, reg == 0);
}
void CPU::rr_r8(const uint8_t opcode, const bool carry)
{
    const uint8_t encodedReg = (opcode & 0x07);
    uint8_t reg = getReg_8(encodedReg);

    uint8_t lsb = (reg & 0x01) != 0 ? 0x80 : 0;
    const uint8_t carryValue = carry && getFlag(CARRY) ? 0x80 : 0;
    reg >>= 1;
    reg += carry ? carryValue : lsb;
    setReg_8(encodedReg, reg);

    setFlag(CARRY, lsb != 0);
    setFlag(HALF_CARRY, false);
    setFlag(SUBTRACTION, false);
    setFlag(ZERO, reg == 0);
}
void CPU::sla_r8(const uint8_t opcode)
{
    const uint8_t encodedReg = (opcode & 0x07);
    uint8_t reg = getReg_8(encodedReg);
    const uint8_t msb = reg & 0x80;
    reg <<= 1;
    setReg_8(encodedReg, reg);

    setFlag(CARRY, msb != 0);
    setFlag(HALF_CARRY, false);
    setFlag(SUBTRACTION, false);
    setFlag(ZERO, reg == 0);
}
void CPU::sra_r8(const uint8_t opcode)
{
    const uint8_t encodedReg = (opcode & 0x07);
    uint8_t reg = getReg_8(encodedReg);

    const uint8_t msb = reg & 0x80;
    const uint8_t lsb = reg & 0x01;
    reg >>= 1;
    reg += msb;
    setReg_8(encodedReg, reg);

    setFlag(CARRY, lsb != 0);
    setFlag(HALF_CARRY, false);
    setFlag(SUBTRACTION, false);
    setFlag(ZERO, reg == 0);
}
void CPU::swap_r8(const uint8_t opcode)
{
    const uint8_t encodedReg = (opcode & 0x07);
    uint8_t reg = getReg_8(encodedReg);

    const uint8_t upper4 = (reg & 0xF0) >> 4;
    const uint8_t lower4 = (reg & 0x0F) << 4;
    reg = upper4 + lower4;
    setReg_8(encodedReg, reg);

    setFlag(CARRY, false);
    setFlag(HALF_CARRY, false);
    setFlag(SUBTRACTION, false);
    setFlag(ZERO, reg == 0);
}
void CPU::srl_r8(const uint8_t opcode)
{
    const uint8_t encodedReg = (opcode & 0x07);
    uint8_t reg = getReg_8(encodedReg);

    const uint8_t lsb = reg & 0x01;
    reg >>= 1;
    setReg_8(encodedReg, reg);

    setFlag(CARRY, lsb != 0);
    setFlag(HALF_CARRY, false);
    setFlag(SUBTRACTION, false);
    setFlag(ZERO, reg == 0);
}
void CPU::bit_b3_r8(const uint8_t opcode)
{
    const uint8_t encodedReg = (opcode & 0x07);
    uint8_t reg = getReg_8(encodedReg);
    const uint8_t bitIndex = (opcode & 0x38) >> 3;
    const uint8_t mask = 0x01 << bitIndex;

    const uint8_t masked = reg & mask;

    setFlag(ZERO, masked == 0);
    setFlag(HALF_CARRY, true);
    setFlag(SUBTRACTION, false);
}
void CPU::res_b3_r8(const uint8_t opcode)
{
    const uint8_t encodedReg = (opcode & 0x07);
    uint8_t reg = getReg_8(encodedReg);
    const uint8_t bitIndex = (opcode & 0x38) >> 3;
    const uint8_t mask = 0x01 << bitIndex;

    reg &= ~mask;
    setReg_8(encodedReg, reg);
}
void CPU::set_b3_r8(const uint8_t opcode)
{
    const uint8_t encodedReg = (opcode & 0x07);
    uint8_t reg = getReg_8(encodedReg);
    const uint8_t bitIndex = (opcode & 0x38) >> 3;
    const uint8_t mask = 0x01 << bitIndex;

    reg |= mask;
    setReg_8(encodedReg, reg);
}
} // namespace gameboy
