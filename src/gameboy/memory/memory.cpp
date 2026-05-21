#include <expected>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "cartridge.hpp"
#include "file.hpp"
#include "memory.hpp"
#include "timer.hpp"

namespace gameboy
{
Memory::Memory(bool cgbMode)
    : cgbMode(cgbMode), bootRom(BOOT_SIZE), vram(VRAM_SIZE * (cgbMode ? 2 : 1)),
      wram(WRAM_SIZE * (cgbMode ? 8 : 2)), oam(OAM_SIZE), hram(HRAM_SIZE),
      ioReg(128)
{
}

uint8_t &Memory::getCell(uint16_t address)
{
    if (address < 0x100)
    {
        return bootRom[address];
    }
    if (address < 0xA000)
    {
        address -= 0x8000;
        address += 1024 * 8 * VBK;
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
        address += 1024 * 4 * SVBK;
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
            address += 1024 * 4 * SVBK;
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
    if (address >= 0xFF80 && address < 0xFFFF)
    {
        address -= 0xFFFF;
        return hram[address];
    }

    throw std::runtime_error("Invalid memory address");
}

void Memory::setBootRom(std::string path)
{
    util::file::loadFile(path, bootRom);
}

void Memory::setCartridge(std::shared_ptr<Cartridge> cartridgePtr)
{
    cartridge = cartridgePtr;
}

std::expected<uint8_t, std::string> Memory::read(uint16_t address)
{
    if (((address < 0x100) && (BootEnabled != 0)) ||
        (address > 0xFF && address < 0x8000) ||
        (address > 0x9FFF && address < 0xC000))
        return cartridge->read(address);

    if (address == 0xFFFF || (address > 0xFEFF && address < 0xFF80))
        return readRegister(address);

    uint8_t value = getCell(address);
    return value;
}

void Memory::write(uint16_t address, const uint8_t value)
{
    if (((address < 0x100) && (BootEnabled != 0)) ||
        (address > 0xFF && address < 0x8000) ||
        (address > 0x9FFF && address < 0xC000))
    {
        cartridge->write(address, value);
        return;
    }

    if (address == 0xFFFF || (address > 0xFEFF && address < 0xFF80))
    {
        writeRegister(address, value);
        return;
    }

    uint8_t &cell = getCell(address);
    cell = value;
}

std::expected<uint8_t, std::string> Memory::readRegister(uint16_t address)
{
    switch (address)
    {
    case 0xFF00:
        return JOYP;
    case 0xFF01:
        return std::unexpected("TODO: SB\n");
    case 0xFF02:
        return std::unexpected("TODO: SC\n");
    case 0xFF04:
        return static_cast<uint8_t>(DIV >> 8);
    case 0xFF05:
        return TIMA;
    case 0xFF06:
        return TMA;
    case 0xFF07:
        return TAC;
    case 0xFF0F:
        return IF;
    case 0xFF10:
        return std::unexpected("TODO: NR10\n");
    case 0xFF11:
        return std::unexpected("TODO: NR11\n");
    case 0xFF12:
        return std::unexpected("TODO: NR12\n");
    case 0xFF13:
        return std::unexpected("TODO: NR13\n");
    case 0xFF14:
        return std::unexpected("TODO: NR14\n");
    case 0xFF16:
        return std::unexpected("TODO: NR21\n");
    case 0xFF17:
        return std::unexpected("TODO: NR22\n");
    case 0xFF18:
        return std::unexpected("TODO: NR23\n");
    case 0xFF19:
        return std::unexpected("TODO: NR24\n");
    case 0xFF1A:
        return std::unexpected("TODO: NR30\n");
    case 0xFF1B:
        return std::unexpected("TODO: NR31\n");
    case 0xFF1C:
        return std::unexpected("TODO: NR32\n");
    case 0xFF1D:
        return std::unexpected("TODO: NR33\n");
    case 0xFF1E:
        return std::unexpected("TODO: NR34\n");
    case 0xFF20:
        return std::unexpected("TODO: NR41\n");
    case 0xFF21:
        return std::unexpected("TODO: NR42\n");
    case 0xFF22:
        return std::unexpected("TODO: NR43\n");
    case 0xFF23:
        return std::unexpected("TODO: NR44\n");
    case 0xFF24:
        return std::unexpected("TODO: NR50\n");
    case 0xFF25:
        return std::unexpected("TODO: NR51\n");
    case 0xFF26:
        return std::unexpected("TODO: NR52\n");
    case 0xFF40:
        return LCDC;
    case 0xFF41:
        return STAT;
    case 0xFF42:
        return SCY;
    case 0xFF43:
        return SCX;
    case 0xFF44:
        return LY;
    case 0xFF45:
        return LYC;
    case 0xFF46:
        return DMA;
    case 0xFF47:
        if (cgbMode)
            return std::unexpected("TODO: non-CGB register BGP\n");

        return BGP;
    case 0xFF48:
        if (cgbMode)
            return std::unexpected("TODO: non-CGB register OBP0\n");

        return OBP0;
    case 0xFF49:
        if (cgbMode)
            return std::unexpected("TODO: non-CGB register OBP1\n");

        return OBP1;
    case 0xFF4A:
        return WY;
    case 0xFF4B:
        return WX;
    case 0xFF4D:
        if (!cgbMode)
            return std::unexpected("ERROR: CGB register KEY1\n");

        return KEY1;
    case 0xFF4F:
        if (!cgbMode)
            return std::unexpected("ERROR: CGB register VBK\n");

        return VBK;
    case 0xFF51:
        if (!cgbMode)
            return std::unexpected("ERROR: CGB register HDMA1\n");

        return std::unexpected("TODO: write-only register HDMA1\n");
        // TODO: check actual behaviour
        return 0xFF;
    case 0xFF52:
        if (!cgbMode)
            return std::unexpected("ERROR: CGB register HDMA2\n");

        return std::unexpected("TODO: write-only register HDMA2\n");
        // TODO: check actual behaviour
        return 0xFF;
    case 0xFF53:
        if (!cgbMode)
            return std::unexpected("ERROR: CGB register HDMA3\n");

        return std::unexpected("TODO: write-only register HDMA3\n");
        // TODO: check actual behaviour
        return 0xFF;
    case 0xFF54:
        if (!cgbMode)
            return std::unexpected("ERROR: CGB register HDMA4\n");

        return std::unexpected("TODO: write-only register HDMA4\n");
        // TODO: check actual behaviour
        return 0xFF;
    case 0xFF55:
        if (!cgbMode)
            return std::unexpected("ERROR: CGB register HDMA5\n");

        return HDMA5;
    case 0xFF56:
        if (!cgbMode)
            return std::unexpected("ERROR: CGB register RP\n");
        else
            return std::unexpected("TODO: RP\n");

        return 0xFF;
    case 0xFF68:
        if (!cgbMode)
            return std::unexpected("ERROR: CGB register BCPS\n");

        return BCPS;
    case 0xFF69:
        if (!cgbMode)
            return std::unexpected("ERROR: CGB register BCPD\n");

        return BCPD;
    case 0xFF6A:
        if (!cgbMode)
            return std::unexpected("ERROR: CGB register OCPS\n");

        return OCPS;
    case 0xFF6B:
        if (!cgbMode)
            return std::unexpected("ERROR: CGB register OCPD\n");

        return OCPD;
    case 0xFF70:
        if (!cgbMode)
            return std::unexpected("ERROR: CGB register SVBK\n");

        return SVBK;
    case 0xFFFF:
        return IE;
    default:
        return std::unexpected("TODO: unknown register\n");
        return 0;
    }
}

std::expected<void, std::string> Memory::writeRegister(uint16_t address,
                                                       uint8_t value)
{
    switch (address)
    {
    case 0xFF00:
        JOYP = value;
        return {};
    case 0xFF01:
        return std::unexpected("TODO: SB\n");
    case 0xFF02:
        return std::unexpected("TODO: SC\n");
    case 0xFF04:
        DIV = 0;
        return {};
    case 0xFF05:
        TIMA = value;
        return {};
    case 0xFF06:
        TMA = value;
        return {};
    case 0xFF07:
        TAC = value;
        return {};
    case 0xFF0F:
        // TODO: investigate behaviour
        IF = value;
        return {};
    case 0xFF10:
        return std::unexpected("TODO: NR10\n");
    case 0xFF11:
        return std::unexpected("TODO: NR11\n");
    case 0xFF12:
        return std::unexpected("TODO: NR12\n");
    case 0xFF13:
        return std::unexpected("TODO: NR13\n");
    case 0xFF14:
        return std::unexpected("TODO: NR14\n");
    case 0xFF16:
        return std::unexpected("TODO: NR21\n");
    case 0xFF17:
        return std::unexpected("TODO: NR22\n");
    case 0xFF18:
        return std::unexpected("TODO: NR23\n");
    case 0xFF19:
        return std::unexpected("TODO: NR24\n");
    case 0xFF1A:
        return std::unexpected("TODO: NR30\n");
    case 0xFF1B:
        return std::unexpected("TODO: NR31\n");
    case 0xFF1C:
        return std::unexpected("TODO: NR32\n");
    case 0xFF1D:
        return std::unexpected("TODO: NR33\n");
    case 0xFF1E:
        return std::unexpected("TODO: NR34\n");
    case 0xFF20:
        return std::unexpected("TODO: NR41\n");
    case 0xFF21:
        return std::unexpected("TODO: NR42\n");
    case 0xFF22:
        return std::unexpected("TODO: NR43\n");
    case 0xFF23:
        return std::unexpected("TODO: NR44\n");
    case 0xFF24:
        return std::unexpected("TODO: NR50\n");
    case 0xFF25:
        return std::unexpected("TODO: NR51\n");
    case 0xFF26:
        return std::unexpected("TODO: NR52\n");
    case 0xFF40:
        LCDC = value;
        return {};
    case 0xFF41:
        value = (value & 0xFC) | STAT;
        STAT = value;
        return {};
    case 0xFF42:
        SCY = value;
        return {};
    case 0xFF43:
        SCX = value;
        return {};
    case 0xFF44:
        return std::unexpected("Attepmted to write on read-only register LY\n");
        return {};
    case 0xFF45:
        LYC = value;
        return {};
    case 0xFF46:
        // TODO: imlpement DMA transfer
        DMA = value;
        return {};
    case 0xFF47:
        if (cgbMode)
            return std::unexpected("ERROR: non-CGB register BGP\n");
        else
            BGP = value;

        return {};
    case 0xFF48:
        if (cgbMode)
            return std::unexpected("ERROR: non-CGB register OBP0\n");
        else
        {
            value &= 0xFC;
            OBP0 = value;
        }

        return {};
    case 0xFF49:
        if (cgbMode)
            return std::unexpected("ERROR: non-CGB register OBP1\n");
        else
        {
            value &= 0xFC;
            OBP1 = value;
        }

        return {};
    case 0xFF4A:
        WY = value;
        return {};
    case 0xFF4B:
        // TODO: decide about bug behaviour
        WX = value;
        return {};
    case 0xFF4D:
        if (!cgbMode)
            return std::unexpected("ERROR: CGB register KEY1\n");
        else
        {
            value &= 0x1;
            KEY1 = value;
        }

        return {};
    case 0xFF4F:
        if (!cgbMode)
            return std::unexpected("ERROR: CGB register VBK\n");
        else
        {
            value &= 0x1;
            VBK = value;
        }

        return {};
    case 0xFF51:
        if (!cgbMode)
            return std::unexpected("ERROR: CGB register HDMA1\n");
        else // TODO: check write behaviour
            HDMA1 = value;

        return {};
    case 0xFF52:
        if (!cgbMode)
            return std::unexpected("ERROR: CGB register HDMA2\n");
        else
        {
            value &= 0xF0;
            HDMA2 = value;
        }

        return {};
    case 0xFF53:
        if (!cgbMode)
            return std::unexpected("ERROR: CGB register HDMA3\n");
        else
        {
            value &= 0x1F;
            HDMA3 = value;
        }

        return {};
    case 0xFF54:
        if (!cgbMode)
            return std::unexpected("ERROR: CGB register HDMA4\n");
        else
        {
            value &= 0xF0;
            HDMA4 = value;
        }

        return {};
    case 0xFF55:
        if (!cgbMode)
            return std::unexpected("ERROR: CGB register HDMA5\n");
        else
            HDMA5 = value;

        return {};
    case 0xFF56:
        if (!cgbMode)
            return std::unexpected("ERROR: CGB register RP\n");
        else
            return std::unexpected("TODO: RP\n");

        return {};
    case 0xFF68:
        if (!cgbMode)
            return std::unexpected("ERROR: CGB register BCPS\n");
        else
        {
            value &= 0xBF;
            BCPS = value;
        }

        return {};
    case 0xFF69:
        if (!cgbMode)
            return std::unexpected("ERROR: CGB register BCPD\n");

        // TODO: implement
        return {};
    case 0xFF6A:
        if (!cgbMode)
            return std::unexpected("ERROR: CGB register OCPS\n");

        // TODO: implement
        return {};
    case 0xFF6B:
        if (!cgbMode)
            return std::unexpected("ERROR: CGB register OCPD\n");

        // TODO: implement
        return {};
    case 0xFF70:
        if (!cgbMode)
            return std::unexpected("ERROR: CGB register SVBK\n");
        else
        {
            value &= 0x07;
            SVBK = value;
        }

        return {};
    case 0xFFFF:
        value &= 0x1F;
        IE = value;
        return {};
    default:
        return std::unexpected("TODO: unknown register\n");
    }
}

std::unique_ptr<Timer> Memory::getTimer()
{
    return std::make_unique<Timer>(DIV, TIMA, TMA, TAC);
}

const std::vector<uint8_t> *Memory::getBootRom() { return &bootRom; }

const std::vector<uint8_t> *Memory::getCartridgeRom()
{
    return &cartridge->getRom();
}

const std::vector<uint8_t> *Memory::getVram() { return &vram; }

const std::vector<uint8_t> *Memory::getCartridgeRam()
{
    return &cartridge->getRam();
}

const std::vector<uint8_t> *Memory::getWram() { return &wram; }

const std::vector<uint8_t> *Memory::getOam() { return &oam; }

const std::vector<uint8_t> *Memory::getIOReg()
{
    ioReg = {JOYP,        0,     0,     0,     static_cast<uint8_t>(DIV >> 8),
             TIMA,        TMA,   TAC,   0,     0,
             0,           0,     0,     0,     0,
             IF,          0,     0,     0,     0,
             0,           0,     0,     0,     0,
             0,           0,     0,     0,     0,
             0,           0,     0,     0,     0,
             0,           0,     0,     0,     0,
             0,           0,     0,     0,     0,
             0,           0,     0,     0,     0,
             0,           0,     0,     0,     0,
             0,           0,     0,     0,     0,
             0,           0,     0,     0,     LCDC,
             STAT,        SCY,   SCX,   LY,    LYC,
             DMA,         BGP,   OBP0,  OBP1,  WY,
             WX,          0,     KEY1,  0,     VBK,
             BootEnabled, HDMA1, HDMA2, HDMA3, HDMA4,
             HDMA5,       0,     0,     0,     0,
             0,           0,     0,     0,     0,
             0,           0,     0,     0,     0,
             0,           0,     0,     0,     BCPS,
             BCPD,        OCPS,  OCPD,  0,     0,
             0,           0,     SVBK,  0,     0,
             0,           0,     0,     0,     0,
             0,           0,     0,     0,     0,
             0,           0,     IE};

    return &ioReg;
}

const std::vector<uint8_t> *Memory::getHram() { return &hram; }

} // namespace gameboy
