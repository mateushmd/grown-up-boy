#include <string>
#include <stdexcept>
#include <vector>
#include <memory>

#include "bus.hpp"
#include "logger.hpp"
#include "types.hpp"
#include "cartridge.hpp"

namespace emulator::components
{
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

    byte &Bus::getCell(word address)
    {
        if (address < 0xA000)
        {
            address -= 0x8000;
            address += 1024 * 8 * (*activeVramBank);
            return vram[address];
        }
        if (address < 0xD000)
        {
            address -= 0xC000;
            return wram[address];
        }
        if (address < 0xE000)
        {
            address -= 0xD000;
            address += 1024 * 4 * (*activeWramBank);
            return wram[address];
        }
        if (address < 0xFE00)
        {
            address -= 0x2000;

            if (address < 0xD000)
                address -= 0xC000;
            if (address < 0xE000)
            {
                address -= 0xD000;
                address += 1024 * 4 * (*activeVramBank);
            }

            return wram[address];
        }
        if (address < 0xFEA0)
        {
            address -= 0xFE00;
            return oam[address];
        }
        if (address < 0xFF00)
        {
            // TODO: implement actual behaviour of range 0xFEA0 - 0xFEFF
        }
        if (address < 0xFF80)
        {
            address -= 0xFF00;
            return ioReg[address];
        }
        if (address < 0xFFFF)
        {
            address -= 0xFFFF;
            return hram[address];
        }
        throw "not implemented";
    }

    void Bus::setCartridge(std::shared_ptr<Cartridge> cartridgePtr)
    {
        cartridge = cartridgePtr;
    }

    byte Bus::read(word address)
    {
        if (address < 0x8000 || (address > 0x9FFF && address < 0xC000))
            return cartridge->read(address);

        byte value = getCell(address);
        return value;
    }

    void Bus::write(word address, const byte value)
    {
        if (address < 0x8000 || (address > 0x9FFF && address < 0xC000))
        {
            cartridge->write(address, value);
            return;
        }

        byte &cell = getCell(address);
        cell = value;
    }

    const std::vector<byte> &Bus::getBootRom() const
    {
        return bootRom;
    }

    const std::vector<byte> &Bus::getCartridgeRom() const
    {
        return cartridge->getRom();
    }

    const std::vector<byte> &Bus::getVram() const
    {
        return vram;
    }

    const std::vector<byte> &Bus::getCartridgeRam() const
    {
        return cartridge->getRam();
    }

    const std::vector<byte> &Bus::getWram() const
    {
        return wram;
    }

    const std::vector<byte> &Bus::getOam() const
    {
        return oam;
    }

    const std::vector<byte> &Bus::getIOReg() const
    {
        return ioReg;
    }

    const std::vector<byte> &Bus::getHram() const
    {
        return hram;
    }

}
