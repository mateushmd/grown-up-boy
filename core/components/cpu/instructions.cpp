#include "cpu.hpp"
#include "bus.hpp"

namespace emulator::components
{
    word CPU::fetchWord()
    {
        const byte low = fetch();
        const byte high = fetch();

        return (static_cast<word>(high) << 8) | low;
    }

    void CPU::rla(const bool carry)
    {
        byte msb = (registers.A & 0x80) != 0 ? 1 : 0;
        const byte carryValue = carry && getFlag(CARRY) ? 1 : 0;
        registers.A <<= 1;
        registers.A += carry ? carryValue : msb;

        setFlag(CARRY, msb);
        setFlag(HALF_CARRY, false);
        setFlag(SUBTRACTION, false);
        setFlag(ZERO, false);
    }
    void CPU::rra(const bool carry)
    {
        byte lsb = (registers.A & 0x1) != 0 ? 0x80 : 0;
        const byte carryValue = carry && getFlag(CARRY) ? 0x80 : 0;
        registers.A >>= 1;
        registers.A += carry ? carryValue : lsb;

        setFlag(CARRY, lsb);
        setFlag(HALF_CARRY, false);
        setFlag(SUBTRACTION, false);
        setFlag(ZERO, false);
    }
    void CPU::jr_imm8()
    {
        // TODO: decide what will happen in infinite loops (maybe a quality of life setting)

        const sbyte offset = static_cast<sbyte>(fetch());

        registers.PC += offset;
    }
    void CPU::daa()
    {
        byte adjust = 0;

        if (getFlag(SUBTRACTION))
        {
            if (getFlag(HALF_CARRY))
                adjust -= 0x06;
            if (getFlag(CARRY))
                adjust -= 0x60;
        }
        else
        {
            if (getFlag(HALF_CARRY) || (registers.A & 0x0F) > 0x09)
                adjust += 0x06;
            if (getFlag(CARRY) || registers.A > 0x99)
            {
                adjust += 0x60;
                setFlag(CARRY, true);
            }
        }

        registers.A += adjust;

        setFlag(ZERO, registers.A == 0);
        setFlag(HALF_CARRY, false);
    }
    void CPU::cpl()
    {
        registers.A = ~registers.A;

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
    void CPU::jp_imm16()
    {
        registers.PC = fetchWord();
    }
    void CPU::add_a_imm8(const bool carry)
    {
        const byte value = fetch();
        const byte carryValue = carry && getFlag(CARRY) ? 1 : 0;
        const byte sum = registers.A + value + carryValue;

        setFlag(CARRY, (0xFF - registers.A) < (value + carryValue));
        setFlag(HALF_CARRY, ((value & 0xF) + (registers.A & 0xF)) > 0xF);
        setFlag(SUBTRACTION, false);
        setFlag(ZERO, sum == 0);

        registers.A = sum;
    }
    void CPU::sub_a_imm8(const bool carry)
    {
        const byte value = fetch();
        const byte carryValue = carry && getFlag(CARRY) ? 1 : 0;
        const byte accValue = registers.A;
        const byte sub = accValue - value - carryValue;

        setFlag(CARRY, (value + carryValue) > accValue);
        setFlag(HALF_CARRY, ((value + carryValue) & 0xF) > (accValue & 0xF));
        setFlag(SUBTRACTION, true);
        setFlag(ZERO, sub == 0);

        registers.A = sub;
    }
    void CPU::and_a_imm8()
    {
        const byte value = fetch();
        registers.A &= value;

        setFlag(CARRY, false);
        setFlag(HALF_CARRY, true);
        setFlag(SUBTRACTION, false);
        setFlag(ZERO, registers.A == 0);
    }
    void CPU::jp_hl()
    {
        registers.PC = registers.HL;
    }
    void CPU::add_sp_imm8()
    {
        const sbyte offset = static_cast<sbyte>(fetch());

        registers.SP += offset;
    }
    void CPU::xor_a_imm8()
    {
        const byte value = fetch();
        registers.A ^= value;

        setFlag(CARRY, false);
        setFlag(HALF_CARRY, false);
        setFlag(SUBTRACTION, false);
    }
    void CPU::or_a_imm8()
    {
        const byte value = fetch();
        registers.A |= value;

        setFlag(CARRY, false);
        setFlag(HALF_CARRY, false);
        setFlag(SUBTRACTION, false);
        setFlag(ZERO, registers.A == 0);
    }
    void CPU::cp_a_imm8()
    {
        const byte value = fetch();
        const byte sub = registers.A - value;

        setFlag(CARRY, value > registers.A);
        setFlag(HALF_CARRY, (value & 0xF) > (registers.A & 0xF));
        setFlag(SUBTRACTION, true);
        setFlag(ZERO, sub == 0);
    }
    void CPU::call_imm16()
    {
        const word address = fetchWord();

        const byte low = static_cast<byte>(registers.PC);
        const byte high = static_cast<byte>(registers.PC >> 8);

        registers.SP--;
        bus.write(registers.SP, high);
        registers.SP--;
        bus.write(registers.SP, low);

        registers.PC = address;
    }
    void CPU::ret()
    {
        const byte low = bus.read(registers.SP);
        registers.SP++;
        const byte high = bus.read(registers.SP);
        registers.SP++;

        const word address = (static_cast<word>(high) << 8) + low;

        registers.PC = address;
    }
    void CPU::reti()
    {
        IME = 1;

        const byte low = bus.read(registers.SP);
        registers.SP++;
        const byte high = bus.read(registers.SP);
        registers.SP++;

        const word address = (static_cast<word>(high) << 8) + low;

        registers.PC = address;
    }
    void CPU::halt()
    {
        // TODO: what halt does?
        state = States::HALT;
        // NOTE: temporary behaviour for testing
        onHalt.notify();
    }
    void CPU::stop()
    {
        state = States::STOP;
    }
    void CPU::ei()
    {
        IME = 1;
    }
    void CPU::di()
    {
        IME = 0;
    }
    void CPU::inc_r16(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x30) >> 4;
        word &reg = getReg_16(encodedReg, R16ReadSource::DEFAULT);
        reg += 1;
    }
    void CPU::inc_r8(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x38) >> 3;
        byte &reg = getReg_8(encodedReg);

        setFlag(HALF_CARRY, ((reg & 0xF) + 1) > 0xF);
        setFlag(SUBTRACTION, false);
        setFlag(ZERO, (reg + 1) == 0);

        reg += 1;
    }
    void CPU::dec_r8(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x38) >> 3;
        byte &reg = getReg_8(encodedReg);

        setFlag(HALF_CARRY, (reg & 0xF) == 0);
        setFlag(SUBTRACTION, true);
        setFlag(ZERO, (reg - 1) == 0);

        reg -= 1;
    }
    void CPU::ld_r8_imm8(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x38) >> 3;
        const byte immediateByte = fetch();
        getReg_8(encodedReg) = immediateByte;
    }
    void CPU::dec_r16(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x30) >> 3;
        word &reg = getReg_16(encodedReg, R16ReadSource::DEFAULT);
        reg -= 1;
    }
    void CPU::ld_r8_r8(const byte opcode)
    {
        const byte encodedDest = (opcode & 0x38) >> 3;
        const byte encodedSrc = (opcode & 0x07);
        const byte srcValue = getReg_8(encodedSrc);
        getReg_8(encodedDest) = srcValue;
    }
    void CPU::add_a_r8(const byte opcode, const bool carry)
    {
        const byte encodedReg = (opcode & 0x07);
        const byte value = getReg_8(encodedReg);

        const byte carryValue = carry && getFlag(CARRY) ? 1 : 0;
        const byte sum = registers.A + value + carryValue;

        setFlag(CARRY, (0xFF - registers.A) < (value + carryValue));
        setFlag(HALF_CARRY, ((value & 0xF) + (registers.A & 0xF)) > 0xF);
        setFlag(SUBTRACTION, false);
        setFlag(ZERO, sum == 0);

        registers.A = sum;
    }
    void CPU::sub_a_r8(const byte opcode, const bool carry)
    {
        const byte encodedReg = (opcode & 0x07);
        const byte value = getReg_8(encodedReg);

        const byte carryValue = carry && getFlag(CARRY) ? 1 : 0;
        const byte sub = registers.A - value - carryValue;

        setFlag(CARRY, (value + carryValue) > registers.A);
        setFlag(HALF_CARRY, ((value + carryValue) & 0xF) > (registers.A & 0xF));
        setFlag(SUBTRACTION, true);
        setFlag(ZERO, sub == 0);

        registers.A = sub;
    }
    void CPU::and_a_r8(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x07);
        const byte value = getReg_8(encodedReg);

        registers.A &= value;

        setFlag(CARRY, false);
        setFlag(HALF_CARRY, true);
        setFlag(SUBTRACTION, false);
        setFlag(ZERO, registers.A == 0);
    }
    void CPU::xor_a_r8(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x07);
        const byte value = getReg_8(encodedReg);

        registers.A ^= value;

        setFlag(CARRY, false);
        setFlag(HALF_CARRY, false);
        setFlag(SUBTRACTION, false);
        setFlag(ZERO, registers.A == 0);
    }
    void CPU::or_a_r8(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x07);
        const byte value = getReg_8(encodedReg);

        registers.A |= value;

        setFlag(CARRY, false);
        setFlag(HALF_CARRY, false);
        setFlag(SUBTRACTION, false);
        setFlag(ZERO, registers.A == 0);
    }
    void CPU::cp_a_r8(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x07);
        const byte value = getReg_8(encodedReg);

        const byte sub = registers.A - value;

        setFlag(CARRY, value > registers.A);
        setFlag(HALF_CARRY, (value & 0xF) > (registers.A & 0xF));
        setFlag(SUBTRACTION, true);
        setFlag(ZERO, sub == 0);
    }
    void CPU::jp_cond_imm16(const byte opcode)
    {
        const byte encodedCondition = (opcode & 0x18) >> 3;
        const word value = fetchWord();

        if (getCondition(encodedCondition))
            registers.PC = value;
    }
    void CPU::jr_cond_imm8(const byte opcode)
    {
        const byte encodedCondition = (opcode & 0x18) >> 3;
        const sbyte offset = static_cast<sbyte>(fetch());

        if (getCondition(encodedCondition))
            registers.PC += offset;
    }
    void CPU::call_cond_imm16(const byte opcode)
    {
        const word address = fetchWord();

        const byte encodedCondition = (opcode & 0x18) >> 3;

        if (!getCondition(encodedCondition))
            return;

        const byte low = static_cast<byte>(registers.PC);
        const byte high = static_cast<byte>(registers.PC >> 8);

        registers.SP--;
        bus.write(registers.SP, high);
        registers.SP--;
        bus.write(registers.SP, low);

        registers.PC = address;
    }
    void CPU::ret_cond(const byte opcode)
    {
        const byte encodedCondition = (opcode & 0x18) >> 3;

        if (!getCondition(encodedCondition))
            return;

        const byte low = bus.read(registers.SP);
        registers.SP++;
        const byte high = bus.read(registers.SP);
        registers.SP++;

        const word address = (static_cast<word>(high) << 8) + low;

        registers.PC = address;
    }
    void CPU::add_hl_r16(const byte opcode)
    {
        const byte encoded = (opcode & 0x30) >> 4;
        const word reg = getReg_16(encoded, R16ReadSource::DEFAULT);
        const word sum = reg + registers.HL;

        setFlag(CARRY, reg > (0xFFFF - registers.HL));
        setFlag(HALF_CARRY, ((registers.HL & 0xFFF) + (reg & 0xFFF)) > 0xFFF);
        setFlag(SUBTRACTION, false);

        registers.HL = sum;
    }
    void CPU::push_r16(const byte opcode)
    {
        const byte encoded = (opcode & 0x30) >> 4;
        const word reg = getReg_16(encoded, R16ReadSource::STACK);
        const byte low = static_cast<byte>(reg);
        const byte high = static_cast<byte>(reg >> 8);

        registers.SP--;
        bus.write(registers.SP, high);
        registers.SP--;
        bus.write(registers.SP, low);
    }
    void CPU::pop_r16(const byte opcode)
    {
        const byte encoded = (opcode & 0x30) >> 4;

        const byte low = bus.read(registers.SP);
        registers.SP++;
        const byte high = bus.read(registers.SP);
        registers.SP++;

        const word value = (static_cast<word>(high) << 8) | low;

        getReg_16(encoded, R16ReadSource::STACK) = value;
    }
    void CPU::rst_tgt3(const byte opcode)
    {
        registers.PC = opcode & 0x38;
    }
    void CPU::ld_r16_imm16(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x30) >> 4;
        word &reg = getReg_16(encodedReg, R16ReadSource::DEFAULT);
        reg = fetchWord();
    }
    void CPU::ld_r16mem_a(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x30) >> 4;
        const word reg = getReg_16(encodedReg, R16ReadSource::MEMORY);
        bus.write(reg, registers.A);
    }
    void CPU::ld_a_r16mem(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x30) >> 4;
        const word reg = getReg_16(encodedReg, R16ReadSource::MEMORY);
        registers.A = bus.read(reg);
    }
    void CPU::ld_imm16mem_sp()
    {
        const word address = fetchWord();
        registers.SP = bus.read(address);
    }
    void CPU::ld_imm16mem_a()
    {
        const word address = fetchWord();
        bus.write(address, registers.A);
    }
    void CPU::ldh_imm8mem_a()
    {
        const byte address = fetch();
        bus.write(0xFF00 + address, registers.A);
    }
    void CPU::ldh_c_a()
    {
        bus.write(0xFF00 + registers.C, registers.A);
    }
    void CPU::ldh_a_imm8mem()
    {
        const byte address = fetch();
        registers.A = bus.read(0xFF00 + address);
    }
    void CPU::ldh_a_c()
    {
        registers.A = bus.read(0xFF00 + registers.C);
    }
    void CPU::ld_a_imm16mem()
    {
        const word address = fetchWord();
        registers.A = bus.read(address);
    }
    void CPU::ld_sp_imm16()
    {
        const word value = fetchWord();
        registers.SP = value;
    }
    void CPU::ld_hl_sp_p_e8()
    {
        const sbyte offset = static_cast<sbyte>(fetch());
        registers.HL = registers.SP + offset;
    }
    void CPU::ld_sp_hl()
    {
        registers.SP = registers.HL;
    }

    void CPU::rl_r8(const byte opcode, const bool carry)
    {
        const byte encodedReg = (opcode & 0x07);
        byte &reg = getReg_8(encodedReg);

        byte msb = (reg & 0x80) != 0 ? 1 : 0;
        const byte carryValue = carry && getFlag(CARRY) ? 1 : 0;
        reg <<= 1;
        reg += carry ? carryValue : msb;

        setFlag(CARRY, msb != 0);
        setFlag(HALF_CARRY, false);
        setFlag(SUBTRACTION, false);
        setFlag(ZERO, reg == 0);
    }
    void CPU::rr_r8(const byte opcode, const bool carry)
    {
        const byte encodedReg = (opcode & 0x07);
        byte &reg = getReg_8(encodedReg);

        byte lsb = (reg & 0x01) != 0 ? 0x80 : 0;
        const byte carryValue = carry && getFlag(CARRY) ? 0x80 : 0;
        reg >>= 1;
        reg += carry ? carryValue : lsb;

        setFlag(CARRY, lsb != 0);
        setFlag(HALF_CARRY, false);
        setFlag(SUBTRACTION, false);
        setFlag(ZERO, reg == 0);
    }
    void CPU::sla_r8(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x07);
        byte &reg = getReg_8(encodedReg);
        const byte msb = reg & 0x80;
        reg <<= 1;

        setFlag(CARRY, msb != 0);
        setFlag(HALF_CARRY, false);
        setFlag(SUBTRACTION, false);
        setFlag(ZERO, reg == 0);
    }
    void CPU::sra_r8(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x07);
        byte &reg = getReg_8(encodedReg);

        const byte msb = reg & 0x80;
        const byte lsb = reg & 0x01;
        reg >>= 1;
        reg += msb;

        setFlag(CARRY, lsb != 0);
        setFlag(HALF_CARRY, false);
        setFlag(SUBTRACTION, false);
        setFlag(ZERO, reg == 0);
    }
    void CPU::swap_r8(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x07);
        byte &reg = getReg_8(encodedReg);

        const byte upper4 = (reg & 0xF0) >> 4;
        const byte lower4 = (reg & 0x0F) << 4;

        reg = upper4 + lower4;

        setFlag(CARRY, false);
        setFlag(HALF_CARRY, false);
        setFlag(SUBTRACTION, false);
        setFlag(ZERO, reg == 0);
    }
    void CPU::srl_r8(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x07);
        byte &reg = getReg_8(encodedReg);

        const byte lsb = reg & 0x01;
        reg >>= 1;

        setFlag(CARRY, lsb != 0);
        setFlag(HALF_CARRY, false);
        setFlag(SUBTRACTION, false);
        setFlag(ZERO, reg == 0);
    }
    void CPU::bit_b3_r8(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x07);
        byte &reg = getReg_8(encodedReg);
        const byte bitIndex = (opcode & 0x38) >> 3;
        const byte mask = 0x01 << bitIndex;

        const byte masked = reg & mask;

        setFlag(ZERO, masked == 0);
        setFlag(HALF_CARRY, true);
        setFlag(SUBTRACTION, false);
    }
    void CPU::res_b3_r8(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x07);
        byte &reg = getReg_8(encodedReg);
        const byte bitIndex = (opcode & 0x38) >> 3;
        const byte mask = 0x01 << bitIndex;

        reg &= ~mask;
    }
    void CPU::set_b3_r8(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x07);
        byte &reg = getReg_8(encodedReg);
        const byte bitIndex = (opcode & 0x38) >> 3;
        const byte mask = 0x01 << bitIndex;

        reg |= mask;
    }
}