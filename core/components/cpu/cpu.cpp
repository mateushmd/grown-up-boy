#include <iostream>
#include <cmath>

#include "cpu.hpp"

namespace emulator::components
{
    CPU::CPU(Bus &bus) : bus(bus)
    {
        boot();
    }

    void CPU::boot()
    {
        registers.IE = 0;
        registers.pairs.AF = 0;
        registers.pairs.BC = 0;
        registers.pairs.DE = 0;
        registers.pairs.HL = 0;
        registers.PC = 0;
        registers.SP = 0xFFFE;

        // TODO: check IME initial state
        IME = 0;

        cbFlag = false;
    }

    // clang-format off
    byte &CPU::getReg_8(const byte encoded)
    {
        switch (encoded)
        {
        case 0x0: return registers.singles.B;
        case 0x1: return registers.singles.C;
        case 0x2: return registers.singles.D;
        case 0x3: return registers.singles.E;
        case 0x4: return registers.singles.H;
        case 0x5: return registers.singles.L;
        case 0x6: return bus.getCell(registers.pairs.HL);
        case 0x7: return registers.singles.A;
        case 0x8: return registers.singles.F;
        default: throw std::invalid_argument("Invalid register code");
        }
    }
    // clang-format on

    word &CPU::getReg_16(const byte encoded, const R16ReadSource source)
    {
        switch (encoded)
        {
        case 0x0:
            return registers.pairs.BC;
        case 0x1:
            return registers.pairs.DE;
        case 0x2:
        {
            if (source == R16ReadSource::MEMORY)
                registers.pairs.HL++;

            return registers.pairs.HL;
        }
        case 0x3:
        {
            if (source == R16ReadSource::DEFAULT)
                return registers.SP;
            else if (source == R16ReadSource::STACK)
                return registers.pairs.AF;
            else
            {
                registers.pairs.HL--;
                return registers.pairs.HL;
            }
        }
        }
    }

    bool CPU::getFlag(const byte flag)
    {
        if (flag < 4 || flag > 7)
            throw std::invalid_argument("Invalid flag code");

        byte mask = 1 << (flag - 4);
        return registers.singles.F & mask;
    }

    void CPU::setFlag(const byte flag, const bool newState)
    {
        if (flag < 4 || flag > 7)
            throw std::invalid_argument("Invalid flag code");

        byte mask = 1 << (flag - 4);

        if (newState)
            registers.singles.F |= mask;
        else
            registers.singles.F &= ~mask;
    }

    bool CPU::getCondition(const byte encoded)
    {
        switch (encoded)
        {
        case 0:
            return !getFlag(ZERO);
        case 1:
            return getFlag(ZERO);
        case 2:
            return !getFlag(CARRY);
        case 3:
            return getFlag(CARRY);
        }
    }

    byte CPU::fetch()
    {
        cbFlag = false;

        byte fetchedByte = bus.read(registers.PC++);
        onFetch.notify(fetchedByte, registers.PC);

        if (state == States::FETCH && fetchedByte == 0xCB)
        {
            cbFlag = true;
            fetchedByte = bus.read(registers.PC++);
            onFetch.notify(fetchedByte, registers.PC);
        }

        state = States::EXECUTE;

        return fetchedByte;
    }

    void CPU::execute(const byte opcode)
    {
        doExecute(opcode);

        // TODO: confirm if this is accurate
        if (state == States::EXECUTE)
            state = States::FETCH;
    }

    // clang-format off
    void CPU::doExecute(const byte opcode)
    {
    if(cbFlag)
    {
        switch(opcode)
        {
        case 0x00: case 0x01: case 0x02: case 0x03: 
        case 0x04: case 0x05: case 0x06: case 0x07: rl_r8(opcode, false);       return;
        case 0x08: case 0x09: case 0x0a: case 0x0b: 
        case 0x0c: case 0x0d: case 0x0e: case 0x0f: rr_r8(opcode, false);       return;
        case 0x10: case 0x11: case 0x12: case 0x13: 
        case 0x14: case 0x15: case 0x16: case 0x17: rl_r8(opcode, true);        return;
        case 0x18: case 0x19: case 0x1a: case 0x1b: 
        case 0x1c: case 0x1d: case 0x1e: case 0x1f: rr_r8(opcode, true);        return;
        case 0x20: case 0x21: case 0x22: case 0x23: 
        case 0x24: case 0x25: case 0x26: case 0x27: sla_r8(opcode);             return;
        case 0x28: case 0x29: case 0x2a: case 0x2b: 
        case 0x2c: case 0x2d: case 0x2e: case 0x2f: sra_r8(opcode);             return;
        case 0x30: case 0x31: case 0x32: case 0x33: 
        case 0x34: case 0x35: case 0x36: case 0x37: swap_r8(opcode);            return;
        case 0x38: case 0x39: case 0x3a: case 0x3b: 
        case 0x3c: case 0x3d: case 0x3e: case 0x3f: srl_r8(opcode);             return;
        case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47: 
        case 0x48: case 0x49: case 0x4a: case 0x4b: case 0x4c: case 0x4d: case 0x4e: case 0x4f: 
        case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57: 
        case 0x58: case 0x59: case 0x5a: case 0x5b: case 0x5c: case 0x5d: case 0x5e: case 0x5f: 
        case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: case 0x67: 
        case 0x68: case 0x69: case 0x6a: case 0x6b: case 0x6c: case 0x6d: case 0x6e: case 0x6f: 
        case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77: 
        case 0x78: case 0x79: case 0x7a: case 0x7b: case 0x7c: case 0x7d: case 0x7e: case 0x7f: 
                                                    bit_b3_r8(opcode);          return; 
        case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87: 
        case 0x88: case 0x89: case 0x8a: case 0x8b: case 0x8c: case 0x8d: case 0x8e: case 0x8f: 
        case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97: 
        case 0x98: case 0x99: case 0x9a: case 0x9b: case 0x9c: case 0x9d: case 0x9e: case 0x9f: 
        case 0xa0: case 0xa1: case 0xa2: case 0xa3: case 0xa4: case 0xa5: case 0xa6: case 0xa7: 
        case 0xa8: case 0xa9: case 0xaa: case 0xab: case 0xac: case 0xad: case 0xae: case 0xaf: 
        case 0xb0: case 0xb1: case 0xb2: case 0xb3: case 0xb4: case 0xb5: case 0xb6: case 0xb7: 
        case 0xb8: case 0xb9: case 0xba: case 0xbb: case 0xbc: case 0xbd: case 0xbe: case 0xbf: 
                                                    res_b3_r8(opcode);          return;
        case 0xc0: case 0xc1: case 0xc2: case 0xc3: case 0xc4: case 0xc5: case 0xc6: case 0xc7: 
        case 0xc8: case 0xc9: case 0xca: case 0xcb: case 0xcc: case 0xcd: case 0xce: case 0xcf: 
        case 0xd0: case 0xd1: case 0xd2: case 0xd3: case 0xd4: case 0xd5: case 0xd6: case 0xd7: 
        case 0xd8: case 0xd9: case 0xda: case 0xdb: case 0xdc: case 0xdd: case 0xde: case 0xdf: 
        case 0xe0: case 0xe1: case 0xe2: case 0xe3: case 0xe4: case 0xe5: case 0xe6: case 0xe7: 
        case 0xe8: case 0xe9: case 0xea: case 0xeb: case 0xec: case 0xed: case 0xee: case 0xef: 
        case 0xf0: case 0xf1: case 0xf2: case 0xf3: case 0xf4: case 0xf5: case 0xf6: case 0xf7: 
        case 0xf8: case 0xf9: case 0xfa: case 0xfb: case 0xfc: case 0xfd: case 0xfe: case 0xff: 
                                                    set_b3_r8(opcode);          return;
        }
    }

        switch (opcode)
        {
        case 0x00:                                                              return;
        case 0x01: case 0x11: case 0x21: case 0x31: ld_r16_imm16(opcode);       return;
        case 0x02: case 0x12: case 0x22: case 0x32: ld_r16mem_a(opcode);        return;
        case 0x0a: case 0x1a: case 0x2a: case 0x3a: ld_a_r16mem(opcode);        return;
        case 0x08:                                  ld_imm16mem_sp();           return;
        case 0x03: case 0x13: case 0x23: case 0x33: inc_r16(opcode);            return;
        case 0x0b: case 0x1b: case 0x2b: case 0x3b: dec_r16(opcode);            return;
        case 0x09: case 0x19: case 0x29: case 0x39: add_hl_r16(opcode);         return;
        case 0x04: case 0x0c: case 0x14: case 0x1c: 
        case 0x24: case 0x2c: case 0x34: case 0x3c: inc_r8(opcode);             return;
        case 0x05: case 0x0d: case 0x15: case 0x1d: 
        case 0x25: case 0x2d: case 0x35: case 0x3d: dec_r8(opcode);             return;
        case 0x06: case 0x0e: case 0x16: case 0x1e: 
        case 0x26: case 0x2e: case 0x36: case 0x3e: ld_r8_imm8(opcode);         return;
        case 0x07:                                  rla(false);                 return;
        case 0x0F:                                  rra(false);                 return;
        case 0x17:                                  rla(true);                  return;
        case 0x1F:                                  rra(true);                  return;
        case 0x27:                                  daa();                      return;
        case 0x2F:                                  cpl();                      return;
        case 0x37:                                  scf();                      return;
        case 0x3F:                                  ccf();                      return;
        case 0x18:                                  jr_imm8();                  return;
        case 0x20: case 0x28: case 0x30: case 0x38: jr_cond_imm8(opcode);       return;
        case 0x10:                                  stop();                     return;
        case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47: 
        case 0x48: case 0x49: case 0x4a: case 0x4b: case 0x4c: case 0x4d: case 0x4e: case 0x4f: 
        case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57: 
        case 0x58: case 0x59: case 0x5a: case 0x5b: case 0x5c: case 0x5d: case 0x5e: case 0x5f: 
        case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: case 0x67: 
        case 0x68: case 0x69: case 0x6a: case 0x6b: case 0x6c: case 0x6d: case 0x6e: case 0x6f: 
        case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x77: 
        case 0x78: case 0x79: case 0x7a: case 0x7b: case 0x7c: case 0x7d: case 0x7e: case 0x7f: 
                                                    ld_r8_r8(opcode);           return;
        case 0x76:                                  halt();                     return;
        case 0x80: case 0x81: case 0x82: case 0x83: 
        case 0x84: case 0x85: case 0x86: case 0x87: add_a_r8(opcode, false);    return;
        case 0x88: case 0x89: case 0x8a: case 0x8b: 
        case 0x8c: case 0x8d: case 0x8e: case 0x8f: add_a_r8(opcode, true);     return;
        case 0x90: case 0x91: case 0x92: case 0x93: 
        case 0x94: case 0x95: case 0x96: case 0x97: sub_a_r8(opcode, false);    return;
        case 0x98: case 0x99: case 0x9a: case 0x9b: 
        case 0x9c: case 0x9d: case 0x9e: case 0x9f: sub_a_r8(opcode, true);     return;
        case 0xa0: case 0xa1: case 0xa2: case 0xa3: 
        case 0xa4: case 0xa5: case 0xa6: case 0xa7: and_a_r8(opcode);           return;
        case 0xa8: case 0xa9: case 0xaa: case 0xab: 
        case 0xac: case 0xad: case 0xae: case 0xaf: xor_a_r8(opcode);           return;
        case 0xb0: case 0xb1: case 0xb2: case 0xb3: 
        case 0xb4: case 0xb5: case 0xb6: case 0xb7: or_a_r8(opcode);            return;
        case 0xb8: case 0xb9: case 0xba: case 0xbb: 
        case 0xbc: case 0xbd: case 0xbe: case 0xbf: cp_a_r8(opcode);            return;
        case 0xc6:                                  add_a_imm8(false);          return;
        case 0xce:                                  add_a_imm8(true);           return;
        case 0xd6:                                  sub_a_imm8(false);          return;
        case 0xde:                                  sub_a_imm8(true);           return;
        case 0xe6:                                  and_a_imm8();               return;
        case 0xee:                                  xor_a_imm8();               return;
        case 0xf6:                                  or_a_imm8();                return;
        case 0xfe:                                  cp_a_imm8();                return;
        case 0xc0: case 0xc8: case 0xd0: case 0xd8: ret_cond(opcode);           return;
        case 0xc9:                                  ret();                      return;
        case 0xd9:                                  reti();                     return;
        case 0xc2: case 0xca: case 0xd2: case 0xda: jp_cond_imm16(opcode);      return;
        case 0xc3:                                  jp_imm16();                 return;
        case 0xe9:                                  jp_hl();                    return;
        case 0xc4: case 0xcc: case 0xd4: case 0xdc: call_cond_imm16(opcode);    return;
        case 0xcd:                                  call_imm16();               return;
        case 0xc7: case 0xcf: case 0xd7: case 0xdf: 
        case 0xe7: case 0xef: case 0xf7: case 0xff: rst_tgt3(opcode);           return;
        case 0xc1: case 0xd1: case 0xe1: case 0xf1: pop_r16(opcode);            return;
        case 0xc5: case 0xd5: case 0xe5: case 0xf5: push_r16(opcode);           return;
        case 0xe2:                                  ldh_c_a();                  return;
        case 0xe0:                                  ldh_imm8mem_a();            return;
        case 0xea:                                  ld_imm16mem_a();            return;
        case 0xf2:                                  ldh_a_c();                  return;
        case 0xf0:                                  ldh_a_imm8mem();            return;
        case 0xfa:                                  ld_a_imm16mem();            return;
        case 0xe8:                                  add_sp_imm8();              return;
        case 0xf8:                                  ld_hl_sp_p_e8();            return;
        case 0xf9:                                  ld_sp_hl();                 return;
        case 0xf3:                                  di();                       return;
        case 0xfb:                                  ei();                       return;
        }
    }
    // clang-format on

    void CPU::clock()
    {
        if (state == States::FETCH)
            fetched = fetch();
        else if (state == States::EXECUTE)
            execute(fetched);
    }
}