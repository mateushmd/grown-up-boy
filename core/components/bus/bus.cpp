#pragma once

#include <string>
#include <stdexcept>
#include <vector>

#include "bus.hpp"
#include "logger.hpp"
#include "types.hpp"
#include "cartridge.hpp"

namespace emulator::components
{
    std::vector<byte> &Bus::getMem(word &address)
    {
        switch (address)
        {
        case 0x0000 ... 0x3FFF:
        {
            if (bootRomEnabled && address < 0x0100)
                return bootRom;

            throw "ROM not implemented";
        }
        case 0x4000 ... 0x7FFF:
        {
            throw "ROM not implemented";
        }
        case 0x8000 ... 0x9FFF:
        {
            address -= 0x8000;
            address += 1024 * 8 * (*activeVramBank);
            return vram;
        }
        case 0xA000 ... 0xBFFF:
        {
            throw "EXTERNAL RAM not implemented";
        }
        case 0xC000 ... 0xCFFF:
        {
            address -= 0xC000;
            return wram;
        }
        case 0xD000 ... 0xDFFF:
        {
            address -= 0xD000;
            address += 1024 * 4 * (*activeWramBank);
            return wram;
        }
        case 0xE000 ... 0xFDFF:
        {
            address -= 0x2000;

            if (address < 0xD000)
                address -= 0xC000;
            if (address < 0xE000)
            {
                address -= 0xD000;
                address += 1024 * 4 * (*activeVramBank);
            }

            return wram;
        }
        case 0xFE00 ... 0xFE9F:
        {
            address -= 0xFE00;
            return oam;
        }
        case 0xFEA0 ... 0xFEFF:
        {
            // TODO: implement actual behaviour of range 0xFEA0 - 0xFEFF
            throw "prohibited";
        }
        case 0xFF00 ... 0xFF7F:
        {
            address -= 0xFF00;
            return ioReg;
        }
        case 0xFF80 ... 0xFFFE:
        {
            address -= 0xFFFF;
            return hram;
        }
        case 0xFFFF:
        {
            throw "not implemented";
        }
        }
    }

    Bus::Bus(bool cgbMode) : cgbMode(cgbMode),
                             bootRom(100),
                             vram(1024 * 8 * (cgbMode ? 2 : 1)),
                             wram(1024 * 4 * (cgbMode ? 8 : 2)),
                             oam(160),
                             hram(127),
                             ioReg(127)
    {
        activeVramBank = &(ioReg[0x004F]);
        activeWramBank = &(ioReg[0x0070]);
        bootRomEnabled = &(ioReg[0x0050]);
    }

    inline byte &Bus::get(word address)
    {
        return getMem(address)[address];
    }

    const byte Bus::read(word address)
    {
        return get(address);
    }

    void Bus::write(word address, const byte value)
    {
        get(address) = value;
    }

    const std::vector<byte> &Bus::getVram() const
    {
        return vram;
    }

    const std::vector<byte> &Bus::getWram() const
    {
        return wram;
    }

    const std::vector<byte> &Bus::getOam() const
    {
        return oam;
    }

    const std::vector<byte> &Bus::getHram() const
    {
        return hram;
    }

    const std::vector<byte> &Bus::getIOReg() const
    {
        return ioReg;
    }
}
