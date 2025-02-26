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
        byte value = A.get();
        byte msb = (value & 0x80) != 0 ? 1 : 0;
        const byte carryValue = carry && getFlag(CARRY) ? 1 : 0;
        value <<= 1;
        value += carry ? carryValue : msb;
        A.set(value);

        setFlag(CARRY, msb);
        setFlag(HALF_CARRY, false);
        setFlag(SUBTRACTION, false);
        setFlag(ZERO, false);
    }
    void CPU::rra(const bool carry)
    {
        byte value = A.get();
        byte lsb = (value & 0x1) != 0 ? 0x80 : 0;
        const byte carryValue = carry && getFlag(CARRY) ? 0x80 : 0;
        value >>= 1;
        value += carry ? carryValue : lsb;
        A.set(value);

        setFlag(CARRY, lsb);
        setFlag(HALF_CARRY, false);
        setFlag(SUBTRACTION, false);
        setFlag(ZERO, false);
    }
    void CPU::jr_imm8()
    {
        // TODO: decide what will happen in infinite loops (maybe a quality of life setting)

        const sbyte offset = static_cast<sbyte>(fetch());

        PC.set(PC.get() + offset);
    }
    void CPU::daa()
    {
        byte value = A.get();
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
            if (getFlag(HALF_CARRY) || (value & 0x0F) > 0x09)
                adjust += 0x06;
            if (getFlag(CARRY) || value > 0x99)
            {
                adjust += 0x60;
                setFlag(CARRY, true);
            }
        }

        value += adjust;
        A.set(value);

        setFlag(ZERO, value == 0);
        setFlag(HALF_CARRY, false);
    }
    void CPU::cpl()
    {
        A.set(~A.get());

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
        PC.set(fetchWord());
    }
    void CPU::add_a_imm8(const bool carry)
    {
        const byte value = fetch();
        byte regValue = A.get();
        const byte carryValue = carry && getFlag(CARRY) ? 1 : 0;
        const byte sum = regValue + value + carryValue;

        setFlag(CARRY, (0xFF - regValue) < (value + carryValue));
        setFlag(HALF_CARRY, ((value & 0xF) + (regValue & 0xF)) > 0xF);
        setFlag(SUBTRACTION, false);
        setFlag(ZERO, sum == 0);

        A.set(sum);
    }
    void CPU::sub_a_imm8(const bool carry)
    {
        const byte value = fetch();
        const byte carryValue = carry && getFlag(CARRY) ? 1 : 0;
        const byte accValue = A.get();
        const byte sub = accValue - value - carryValue;

        setFlag(CARRY, (value + carryValue) > accValue);
        setFlag(HALF_CARRY, ((value + carryValue) & 0xF) > (accValue & 0xF));
        setFlag(SUBTRACTION, true);
        setFlag(ZERO, sub == 0);

        A.set(sub);
    }
    void CPU::and_a_imm8()
    {
        const byte value = fetch();
        A.set(A.get() & value);

        setFlag(CARRY, false);
        setFlag(HALF_CARRY, true);
        setFlag(SUBTRACTION, false);
        setFlag(ZERO, A.get() == 0);
    }
    void CPU::jp_hl()
    {
        PC.set(HL.get());
    }
    void CPU::add_sp_imm8()
    {
        const sbyte offset = static_cast<sbyte>(fetch());

        SP.set(SP.get() + offset);
    }
    void CPU::xor_a_imm8()
    {
        const byte value = fetch();
        A.set(A.get() ^ value);

        setFlag(CARRY, false);
        setFlag(HALF_CARRY, false);
        setFlag(SUBTRACTION, false);
    }
    void CPU::or_a_imm8()
    {
        const byte value = fetch();
        A.set(A.get() | value);

        setFlag(CARRY, false);
        setFlag(HALF_CARRY, false);
        setFlag(SUBTRACTION, false);
        setFlag(ZERO, A.get() == 0);
    }
    void CPU::cp_a_imm8()
    {
        const byte value = fetch();
        byte regValue = A.get();
        const byte sub = regValue - value;

        setFlag(CARRY, value > regValue);
        setFlag(HALF_CARRY, (value & 0xF) > (regValue & 0xF));
        setFlag(SUBTRACTION, true);
        setFlag(ZERO, sub == 0);
    }
    void CPU::call_imm16()
    {
        const word address = fetchWord();

        const byte low = static_cast<byte>(PC.get());
        const byte high = static_cast<byte>(PC.get() >> 8);

        SP.dec();
        bus.write(SP.get(), high);
        SP.dec();
        bus.write(SP.get(), low);

        PC.set(address);
    }
    void CPU::ret()
    {
        const byte low = bus.read(SP.get());
        SP.inc();
        const byte high = bus.read(SP.get());
        SP.inc();

        const word address = (static_cast<word>(high) << 8) + low;

        PC.set(address);
    }
    void CPU::reti()
    {
        IME = 1;

        const byte low = bus.read(SP.get());
        SP.inc();
        const byte high = bus.read(SP.get());
        SP.inc();

        const word address = (static_cast<word>(high) << 8) + low;

        PC.set(address);
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
        setReg_16(encodedReg, getReg_16(encodedReg, R16Source::DEFAULT) + 1, R16Source::DEFAULT);
    }
    void CPU::inc_r8(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x38) >> 3;
        byte reg = getReg_8(encodedReg);

        setFlag(HALF_CARRY, ((reg & 0xF) + 1) > 0xF);
        setFlag(SUBTRACTION, false);
        setFlag(ZERO, (reg + 1) == 0);

        ++reg;
        setReg_8(encodedReg, reg);
    }
    void CPU::dec_r8(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x38) >> 3;
        byte reg = getReg_8(encodedReg);

        setFlag(HALF_CARRY, (reg & 0xF) == 0);
        setFlag(SUBTRACTION, true);
        setFlag(ZERO, (reg - 1) == 0);

        --reg;
        setReg_8(encodedReg, reg);
    }
    void CPU::ld_r8_imm8(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x38) >> 3;
        const byte immediateByte = fetch();
        setReg_8(encodedReg, immediateByte);
    }
    void CPU::dec_r16(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x30) >> 3;
        word reg = getReg_16(encodedReg, R16Source::DEFAULT);
        --reg;
        setReg_16(encodedReg, reg, R16Source::DEFAULT);
    }
    void CPU::ld_r8_r8(const byte opcode)
    {
        const byte encodedDest = (opcode & 0x38) >> 3;
        const byte encodedSrc = (opcode & 0x07);
        const byte srcValue = getReg_8(encodedSrc);
        setReg_8(encodedDest, srcValue);
    }
    void CPU::add_a_r8(const byte opcode, const bool carry)
    {
        const byte encodedReg = (opcode & 0x07);
        const byte value = getReg_8(encodedReg);
        byte aValue = A.get();

        const byte carryValue = carry && getFlag(CARRY) ? 1 : 0;
        const byte sum = aValue + value + carryValue;

        setFlag(CARRY, (0xFF - aValue) < (value + carryValue));
        setFlag(HALF_CARRY, ((value & 0xF) + (aValue & 0xF)) > 0xF);
        setFlag(SUBTRACTION, false);
        setFlag(ZERO, sum == 0);

        A.set(sum);
    }
    void CPU::sub_a_r8(const byte opcode, const bool carry)
    {
        const byte encodedReg = (opcode & 0x07);
        const byte value = getReg_8(encodedReg);
        byte aValue = A.get();

        const byte carryValue = carry && getFlag(CARRY) ? 1 : 0;
        const byte sub = aValue - value - carryValue;

        setFlag(CARRY, (value + carryValue) > aValue);
        setFlag(HALF_CARRY, ((value + carryValue) & 0xF) > (aValue & 0xF));
        setFlag(SUBTRACTION, true);
        setFlag(ZERO, sub == 0);

        A.set(sub);
    }
    void CPU::and_a_r8(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x07);
        const byte value = getReg_8(encodedReg);

        A.set(A.get() & value);

        setFlag(CARRY, false);
        setFlag(HALF_CARRY, true);
        setFlag(SUBTRACTION, false);
        setFlag(ZERO, A.get() == 0);
    }
    void CPU::xor_a_r8(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x07);
        const byte value = getReg_8(encodedReg);

        A.set(A.get() ^ value);

        setFlag(CARRY, false);
        setFlag(HALF_CARRY, false);
        setFlag(SUBTRACTION, false);
        setFlag(ZERO, A.get() == 0);
    }
    void CPU::or_a_r8(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x07);
        const byte value = getReg_8(encodedReg);

        A.set(A.get() | value);

        setFlag(CARRY, false);
        setFlag(HALF_CARRY, false);
        setFlag(SUBTRACTION, false);
        setFlag(ZERO, A.get() == 0);
    }
    void CPU::cp_a_r8(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x07);
        const byte value = getReg_8(encodedReg);
        byte aValue = A.get();
        const byte sub = aValue - value;

        setFlag(CARRY, value > aValue);
        setFlag(HALF_CARRY, (value & 0xF) > (aValue & 0xF));
        setFlag(SUBTRACTION, true);
        setFlag(ZERO, sub == 0);
    }
    void CPU::jp_cond_imm16(const byte opcode)
    {
        const byte encodedCondition = (opcode & 0x18) >> 3;
        const word value = fetchWord();

        if (getCondition(encodedCondition))
        {
            PC.set(value);
            branch = true;
        }
    }
    void CPU::jr_cond_imm8(const byte opcode)
    {
        const byte encodedCondition = (opcode & 0x18) >> 3;
        const sbyte offset = static_cast<sbyte>(fetch());

        if (getCondition(encodedCondition))
        {
            PC.set(PC.get() + offset);
            branch = true;
        }
    }
    void CPU::call_cond_imm16(const byte opcode)
    {
        const word address = fetchWord();

        const byte encodedCondition = (opcode & 0x18) >> 3;

        if (!getCondition(encodedCondition))
            return;

        branch = true;

        const byte low = static_cast<byte>(PC.get());
        const byte high = static_cast<byte>(PC.get() >> 8);

        SP.dec();
        bus.write(SP.get(), high);
        SP.dec();
        bus.write(SP.get(), low);

        PC.set(address);
    }
    void CPU::ret_cond(const byte opcode)
    {
        const byte encodedCondition = (opcode & 0x18) >> 3;

        if (!getCondition(encodedCondition))
            return;

        branch = true;

        const byte low = bus.read(SP.get());
        SP.inc();
        const byte high = bus.read(SP.get());
        SP.inc();

        const word address = (static_cast<word>(high) << 8) + low;

        PC.set(address);
    }
    void CPU::add_hl_r16(const byte opcode)
    {
        const byte encoded = (opcode & 0x30) >> 4;
        const word reg = getReg_16(encoded, R16Source::DEFAULT);
        word hlValue = HL.get();
        const word sum = reg + hlValue;

        setFlag(CARRY, reg > (0xFFFF - hlValue));
        setFlag(HALF_CARRY, ((hlValue & 0xFFF) + (reg & 0xFFF)) > 0xFFF);
        setFlag(SUBTRACTION, false);

        HL.set(sum);
    }
    void CPU::push_r16(const byte opcode)
    {
        const byte encoded = (opcode & 0x30) >> 4;
        const word reg = getReg_16(encoded, R16Source::STACK);
        const byte low = static_cast<byte>(reg);
        const byte high = static_cast<byte>(reg >> 8);

        SP.dec();
        bus.write(SP.get(), high);
        SP.dec();
        bus.write(SP.get(), low);
    }
    void CPU::pop_r16(const byte opcode)
    {
        const byte encoded = (opcode & 0x30) >> 4;

        const byte low = bus.read(SP.get());
        SP.inc();
        const byte high = bus.read(SP.get());
        SP.inc();

        const word value = (static_cast<word>(high) << 8) | low;

        setReg_16(encoded, value, R16Source::STACK);
    }
    void CPU::rst_tgt3(const byte opcode)
    {
        PC.set(opcode & 0x38);
    }
    void CPU::ld_r16_imm16(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x30) >> 4;
        setReg_16(encodedReg, fetchWord(), R16Source::DEFAULT);
    }
    void CPU::ld_r16mem_a(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x30) >> 4;
        const word reg = getReg_16(encodedReg, R16Source::MEMORY);
        bus.write(reg, A.get());
    }
    void CPU::ld_a_r16mem(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x30) >> 4;
        const word reg = getReg_16(encodedReg, R16Source::MEMORY);
        A.set(bus.read(reg));
    }
    void CPU::ld_imm16mem_sp()
    {
        const word address = fetchWord();
        SP.set(bus.read(address));
    }
    void CPU::ld_imm16mem_a()
    {
        const word address = fetchWord();
        bus.write(address, A.get());
    }
    void CPU::ldh_imm8mem_a()
    {
        const byte address = fetch();
        bus.write(0xFF00 + address, A.get());
    }
    void CPU::ldh_c_a()
    {
        bus.write(0xFF00 + C.get(), A.get());
    }
    void CPU::ldh_a_imm8mem()
    {
        const byte address = fetch();
        A.set(bus.read(0xFF00 + address));
    }
    void CPU::ldh_a_c()
    {
        A.set(bus.read(0xFF00 + C.get()));
    }
    void CPU::ld_a_imm16mem()
    {
        const word address = fetchWord();
        A.set(bus.read(address));
    }
    void CPU::ld_sp_imm16()
    {
        const word value = fetchWord();
        SP.set(value);
    }
    void CPU::ld_hl_sp_p_e8()
    {
        const sbyte offset = static_cast<sbyte>(fetch());
        HL.set(SP.get() + offset);
    }
    void CPU::ld_sp_hl()
    {
        SP.set(HL.get());
    }

    void CPU::rl_r8(const byte opcode, const bool carry)
    {
        const byte encodedReg = (opcode & 0x07);
        byte reg = getReg_8(encodedReg);

        byte msb = (reg & 0x80) != 0 ? 1 : 0;
        const byte carryValue = carry && getFlag(CARRY) ? 1 : 0;
        reg <<= 1;
        reg += carry ? carryValue : msb;
        setReg_8(encodedReg, reg);

        setFlag(CARRY, msb != 0);
        setFlag(HALF_CARRY, false);
        setFlag(SUBTRACTION, false);
        setFlag(ZERO, reg == 0);
    }
    void CPU::rr_r8(const byte opcode, const bool carry)
    {
        const byte encodedReg = (opcode & 0x07);
        byte reg = getReg_8(encodedReg);

        byte lsb = (reg & 0x01) != 0 ? 0x80 : 0;
        const byte carryValue = carry && getFlag(CARRY) ? 0x80 : 0;
        reg >>= 1;
        reg += carry ? carryValue : lsb;
        setReg_8(encodedReg, reg);

        setFlag(CARRY, lsb != 0);
        setFlag(HALF_CARRY, false);
        setFlag(SUBTRACTION, false);
        setFlag(ZERO, reg == 0);
    }
    void CPU::sla_r8(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x07);
        byte reg = getReg_8(encodedReg);
        const byte msb = reg & 0x80;
        reg <<= 1;
        setReg_8(encodedReg, reg);

        setFlag(CARRY, msb != 0);
        setFlag(HALF_CARRY, false);
        setFlag(SUBTRACTION, false);
        setFlag(ZERO, reg == 0);
    }
    void CPU::sra_r8(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x07);
        byte reg = getReg_8(encodedReg);

        const byte msb = reg & 0x80;
        const byte lsb = reg & 0x01;
        reg >>= 1;
        reg += msb;
        setReg_8(encodedReg, reg);

        setFlag(CARRY, lsb != 0);
        setFlag(HALF_CARRY, false);
        setFlag(SUBTRACTION, false);
        setFlag(ZERO, reg == 0);
    }
    void CPU::swap_r8(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x07);
        byte reg = getReg_8(encodedReg);

        const byte upper4 = (reg & 0xF0) >> 4;
        const byte lower4 = (reg & 0x0F) << 4;
        reg = upper4 + lower4;
        setReg_8(encodedReg, reg);

        setFlag(CARRY, false);
        setFlag(HALF_CARRY, false);
        setFlag(SUBTRACTION, false);
        setFlag(ZERO, reg == 0);
    }
    void CPU::srl_r8(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x07);
        byte reg = getReg_8(encodedReg);

        const byte lsb = reg & 0x01;
        reg >>= 1;
        setReg_8(encodedReg, reg);

        setFlag(CARRY, lsb != 0);
        setFlag(HALF_CARRY, false);
        setFlag(SUBTRACTION, false);
        setFlag(ZERO, reg == 0);
    }
    void CPU::bit_b3_r8(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x07);
        byte reg = getReg_8(encodedReg);
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
        byte reg = getReg_8(encodedReg);
        const byte bitIndex = (opcode & 0x38) >> 3;
        const byte mask = 0x01 << bitIndex;

        reg &= ~mask;
        setReg_8(encodedReg, reg);
    }
    void CPU::set_b3_r8(const byte opcode)
    {
        const byte encodedReg = (opcode & 0x07);
        byte reg = getReg_8(encodedReg);
        const byte bitIndex = (opcode & 0x38) >> 3;
        const byte mask = 0x01 << bitIndex;

        reg |= mask;
        setReg_8(encodedReg, reg);
    }
}