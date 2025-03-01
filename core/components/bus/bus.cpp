#include <string>
#include <stdexcept>
#include <vector>
#include <memory>

#include "bus.hpp"
#include "logger.hpp"
#include "types.hpp"
#include "cartridge.hpp"
#include "file.hpp"
#include "timer.hpp"

namespace emulator::components
{
    Bus::Bus(bool cgbMode) : cgbMode(cgbMode),
                             bootRom(BOOT_SIZE),
                             vram(VRAM_SIZE * (cgbMode ? 2 : 1)),
                             wram(WRAM_SIZE * (cgbMode ? 8 : 2)),
                             oam(OAM_SIZE),
                             hram(HRAM_SIZE),
                             ioReg(128)
    {
    }

    byte &Bus::getCell(word address)
    {
        if (address < 0x100)
        {
            return bootRom[address];
        }
        if (address < 0xA000)
        {
            address -= 0x8000;
            address += 1024 * 8 * VBK.get();
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
            address += 1024 * 4 * SVBK.get();
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
                address += 1024 * 4 * SVBK.get();
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

    void Bus::setBootRom(std::string path)
    {
        util::file::loadFile(path, bootRom);
    }

    void Bus::setCartridge(std::shared_ptr<Cartridge> cartridgePtr)
    {
        cartridge = cartridgePtr;
    }

    byte Bus::read(word address)
    {
        if (((address < 0x100) && (BootEnabled.get() != 0)) || (address > 0xFF && address < 0x8000) || (address > 0x9FFF && address < 0xC000))
            return cartridge->read(address);

        if (address == 0xFFFF || (address > 0xFEFF && address < 0xFF80))
            return readRegister(address);

        byte value = getCell(address);
        return value;
    }

    void Bus::write(word address, const byte value)
    {
        if (((address < 0x100) && (BootEnabled.get() != 0)) || (address > 0xFF && address < 0x8000) || (address > 0x9FFF && address < 0xC000))
        {
            cartridge->write(address, value);
            return;
        }

        if (address == 0xFFFF || (address > 0xFEFF && address < 0xFF80))
        {
            writeRegister(address, value);
            return;
        }

        byte &cell = getCell(address);
        cell = value;
    }

    byte Bus::readRegister(word address)
    {
        switch (address)
        {
        case 0xFF00:
            return JOYP.get();
        case 0xFF01:
            logger::warning << "Attempted to access SB\n";
            return 0;
        case 0xFF02:
            logger::warning << "Attempted to access SC\n";
            return 0;
        case 0xFF04:
            return static_cast<byte>(DIV.get() >> 8);
        case 0xFF05:
            return TIMA.get();
        case 0xFF06:
            return TMA.get();
        case 0xFF07:
            return TAC.get();
        case 0xFF0F:
            return IF.get();
        case 0xFF10:
            logger::warning << "Attempted to access NR10\n";
            return 0;
        case 0xFF11:
            logger::warning << "Attempted to access NR11\n";
            return 0;
        case 0xFF12:
            logger::warning << "Attempted to access NR12\n";
            return 0;
        case 0xFF13:
            logger::warning << "Attempted to access NR13\n";
            return 0;
        case 0xFF14:
            logger::warning << "Attempted to access NR14\n";
            return 0;
        case 0xFF16:
            logger::warning << "Attempted to access NR21\n";
            return 0;
        case 0xFF17:
            logger::warning << "Attempted to access NR22\n";
            return 0;
        case 0xFF18:
            logger::warning << "Attempted to access NR23\n";
            return 0;
        case 0xFF19:
            logger::warning << "Attempted to access NR24\n";
            return 0;
        case 0xFF1A:
            logger::warning << "Attempted to access NR30\n";
            return 0;
        case 0xFF1B:
            logger::warning << "Attempted to access NR31\n";
            return 0;
        case 0xFF1C:
            logger::warning << "Attempted to access NR32\n";
            return 0;
        case 0xFF1D:
            logger::warning << "Attempted to access NR33\n";
            return 0;
        case 0xFF1E:
            logger::warning << "Attempted to access NR34\n";
            return 0;
        case 0xFF20:
            logger::warning << "Attempted to access NR41\n";
            return 0;
        case 0xFF21:
            logger::warning << "Attempted to access NR42\n";
            return 0;
        case 0xFF22:
            logger::warning << "Attempted to access NR43\n";
            return 0;
        case 0xFF23:
            logger::warning << "Attempted to access NR44\n";
            return 0;
        case 0xFF24:
            logger::warning << "Attempted to access NR50\n";
            return 0;
        case 0xFF25:
            logger::warning << "Attempted to access NR51\n";
            return 0;
        case 0xFF26:
            logger::warning << "Attempted to access NR52\n";
            return 0;
        case 0xFF40:
            return LCDC.get();
        case 0xFF41:
            return STAT.get();
        case 0xFF42:
            return SCY.get();
        case 0xFF43:
            return SCX.get();
        case 0xFF44:
            return LY.get();
        case 0xFF45:
            return LYC.get();
        case 0xFF46:
            return DMA.get();
        case 0xFF47:
            if (cgbMode)
            {
                logger::warning << "Attempted to access non-CGB register BGP\n";
                return 0;
            }

            return BGP.get();
        case 0xFF48:
            if (cgbMode)
            {
                logger::warning << "Attempted to access non-CGB register OBP0\n";
                return 0;
            }

            return OBP0.get();
        case 0xFF49:
            if (cgbMode)
            {
                logger::warning << "Attempted to access non-CGB register OBP1\n";
                return 0;
            }

            return OBP1.get();
        case 0xFF4A:
            return WY.get();
        case 0xFF4B:
            return WX.get();
        case 0xFF4D:
            if (!cgbMode)
            {
                logger::warning << "Attempted to access CGB register KEY1\n";
                return 0xFF;
            }

            return KEY1.get();
        case 0xFF4F:
            if (!cgbMode)
            {
                logger::warning << "Attempted to access CGB register VBK\n";
                return 0xFF;
            }

            return VBK.get();
        case 0xFF51:
            if (!cgbMode)
            {
                logger::warning << "Attempted to access CGB register HDMA1\n";
                return 0xFF;
            }

            logger::warning << "Attempted to access write-only register HDMA1\n";
            // TODO: check actual behaviour
            return 0xFF;
        case 0xFF52:
            if (!cgbMode)
            {
                logger::warning << "Attempted to access CGB register HDMA2\n";
                return 0xFF;
            }

            logger::warning << "Attempted to access write-only register HDMA2\n";
            // TODO: check actual behaviour
            return 0xFF;
        case 0xFF53:
            if (!cgbMode)
            {
                logger::warning << "Attempted to access CGB register HDMA3\n";
                return 0xFF;
            }

            logger::warning << "Attempted to access write-only register HDMA3\n";
            // TODO: check actual behaviour
            return 0xFF;
        case 0xFF54:
            if (!cgbMode)
            {
                logger::warning << "Attempted to access CGB register HDMA4\n";
                return 0xFF;
            }

            logger::warning << "Attempted to access write-only register HDMA4\n";
            // TODO: check actual behaviour
            return 0xFF;
        case 0xFF55:
            if (!cgbMode)
            {
                logger::warning << "Attempted to access CGB register HDMA5\n";
                return 0xFF;
            }

            return HDMA5.get();
        case 0xFF56:
            if (!cgbMode)
                logger::warning << "Attempted to access CGB register RP\n";
            else
                logger::warning << "Attempted to access RP\n";

            return 0xFF;
        case 0xFF68:
            if (!cgbMode)
            {
                logger::warning << "Attempted to access CGB register BCPS\n";
                return 0xFF;
            }

            return BCPS.get();
        case 0xFF69:
            if (!cgbMode)
            {
                logger::warning << "Attempted to access CGB register BCPD\n";
                return 0xFF;
            }

            return BCPD.get();
        case 0xFF6A:
            if (!cgbMode)
            {
                logger::warning << "Attempted to access CGB register OCPS\n";
                return 0xFF;
            }

            return OCPS.get();
        case 0xFF6B:
            if (!cgbMode)
            {
                logger::warning << "Attempted to access CGB register OCPD\n";
                return 0xFF;
            }

            return OCPD.get();
        case 0xFF70:
            if (!cgbMode)
            {
                logger::warning << "Attempted to access CGB register SVBK\n";
                return 0xFF;
            }

            return SVBK.get();
        case 0xFFFF:
            return IE.get();
        default:
            logger::warning << "Attempted to access unknown register\n";
            return 0;
        }
    }

    void Bus::writeRegister(word address, byte value)
    {
        switch (address)
        {
        case 0xFF00:
            JOYP.set(value);
            return;
        case 0xFF01:
            logger::warning << "Attempted to access SB\n";
            return;
        case 0xFF02:
            logger::warning << "Attempted to access SC\n";
            return;
        case 0xFF04:
            DIV.set(0);
            return;
        case 0xFF05:
            TIMA.set(value);
            return;
        case 0xFF06:
            TMA.set(value);
            return;
        case 0xFF07:
            TAC.set(value);
            return;
        case 0xFF0F:
            // TODO: investigate behaviour
            IF.set(value);
            return;
        case 0xFF10:
            logger::warning << "Attempted to access NR10\n";
            return;
        case 0xFF11:
            logger::warning << "Attempted to access NR11\n";
            return;
        case 0xFF12:
            logger::warning << "Attempted to access NR12\n";
            return;
        case 0xFF13:
            logger::warning << "Attempted to access NR13\n";
            return;
        case 0xFF14:
            logger::warning << "Attempted to access NR14\n";
            return;
        case 0xFF16:
            logger::warning << "Attempted to access NR21\n";
            return;
        case 0xFF17:
            logger::warning << "Attempted to access NR22\n";
            return;
        case 0xFF18:
            logger::warning << "Attempted to access NR23\n";
            return;
        case 0xFF19:
            logger::warning << "Attempted to access NR24\n";
            return;
        case 0xFF1A:
            logger::warning << "Attempted to access NR30\n";
            return;
        case 0xFF1B:
            logger::warning << "Attempted to access NR31\n";
            return;
        case 0xFF1C:
            logger::warning << "Attempted to access NR32\n";
            return;
        case 0xFF1D:
            logger::warning << "Attempted to access NR33\n";
            return;
        case 0xFF1E:
            logger::warning << "Attempted to access NR34\n";
            return;
        case 0xFF20:
            logger::warning << "Attempted to access NR41\n";
            return;
        case 0xFF21:
            logger::warning << "Attempted to access NR42\n";
            return;
        case 0xFF22:
            logger::warning << "Attempted to access NR43\n";
            return;
        case 0xFF23:
            logger::warning << "Attempted to access NR44\n";
            return;
        case 0xFF24:
            logger::warning << "Attempted to access NR50\n";
            return;
        case 0xFF25:
            logger::warning << "Attempted to access NR51\n";
            return;
        case 0xFF26:
            logger::warning << "Attempted to access NR52\n";
            return;
        case 0xFF40:
            LCDC.set(value);
            return;
        case 0xFF41:
            value = (value & 0xFC) | STAT.get();
            STAT.set(value);
            return;
        case 0xFF42:
            SCY.set(value);
            return;
        case 0xFF43:
            SCX.set(value);
            return;
        case 0xFF44:
            logger::warning << "Attepmted to write on read-only register LY\n";
            return;
        case 0xFF45:
            LYC.set(value);
            return;
        case 0xFF46:
            // TODO: imlpement DMA transfer
            DMA.set(value);
            return;
        case 0xFF47:
            if (cgbMode)
                logger::warning << "Attempted to access non-CGB register BGP\n";
            else
                BGP.set(value);

            return;
        case 0xFF48:
            if (cgbMode)
                logger::warning << "Attempted to access non-CGB register OBP0\n";
            else
            {
                value &= 0xFC;
                OBP0.set(value);
            }

            return;
        case 0xFF49:
            if (cgbMode)
                logger::warning << "Attempted to access non-CGB register OBP1\n";
            else
            {
                value &= 0xFC;
                OBP1.set(value);
            }

            return;
        case 0xFF4A:
            WY.set(value);
            return;
        case 0xFF4B:
            // TODO: decide about bug behaviour
            WX.set(value);
            return;
        case 0xFF4D:
            if (!cgbMode)
                logger::warning << "Attempted to access CGB register KEY1\n";
            else
            {
                value &= 0x1;
                KEY1.set(value);
            }

            return;
        case 0xFF4F:
            if (!cgbMode)
                logger::warning << "Attempted to access CGB register VBK\n";
            else
            {
                value &= 0x1;
                VBK.set(value);
            }

            return;
        case 0xFF51:
            if (!cgbMode)
                logger::warning << "Attempted to access CGB register HDMA1\n";
            else // TODO: check write behaviour
                HDMA1.set(value);

            return;
        case 0xFF52:
            if (!cgbMode)
                logger::warning << "Attempted to access CGB register HDMA2\n";
            else
            {
                value &= 0xF0;
                HDMA2.set(value);
            }

            return;
        case 0xFF53:
            if (!cgbMode)
                logger::warning << "Attempted to access CGB register HDMA3\n";
            else
            {
                value &= 0x1F;
                HDMA3.set(value);
            }

            return;
        case 0xFF54:
            if (!cgbMode)
                logger::warning << "Attempted to access CGB register HDMA4\n";
            else
            {
                value &= 0xF0;
                HDMA4.set(value);
            }

            return;
        case 0xFF55:
            if (!cgbMode)
                logger::warning << "Attempted to access CGB register HDMA5\n";
            else
                HDMA5.set(value);

            return;
        case 0xFF56:
            if (!cgbMode)
                logger::warning << "Attempted to access CGB register RP\n";
            else
                logger::warning << "Attempted to access RP\n";

            return;
        case 0xFF68:
            if (!cgbMode)
                logger::warning << "Attempted to access CGB register BCPS\n";
            else
            {
                value &= 0xBF;
                BCPS.set(value);
            }

            return;
        case 0xFF69:
            if (!cgbMode)
                logger::warning << "Attempted to access CGB register BCPD\n";

            // TODO: implement
            return;
        case 0xFF6A:
            if (!cgbMode)
                logger::warning << "Attempted to access CGB register OCPS\n";

            // TODO: implement
            return;
        case 0xFF6B:
            if (!cgbMode)
                logger::warning << "Attempted to access CGB register OCPD\n";

            // TODO: implement
            return;
        case 0xFF70:
            if (!cgbMode)
                logger::warning << "Attempted to access CGB register SVBK\n";
            else
            {
                value &= 0x07;
                SVBK.set(value);
            }

            return;
        case 0xFFFF:
            value &= 0x1F;
            IE.set(value);
            return;
        default:
            logger::warning << "Attempted to access unknown register\n";
        }
    }

    std::unique_ptr<Timer> Bus::getTimer()
    {
        return std::make_unique<Timer>(DIV, TIMA, TMA, TAC);
    }

    const std::vector<byte> *Bus::getBootRom()
    {
        return &bootRom;
    }

    const std::vector<byte> *Bus::getCartridgeRom()
    {
        return &cartridge->getRom();
    }

    const std::vector<byte> *Bus::getVram()
    {
        return &vram;
    }

    const std::vector<byte> *Bus::getCartridgeRam()
    {
        return &cartridge->getRam();
    }

    const std::vector<byte> *Bus::getWram()
    {
        return &wram;
    }

    const std::vector<byte> *Bus::getOam()
    {
        return &oam;
    }

    const std::vector<byte> *Bus::getIOReg()
    {
        ioReg = {
            JOYP.get(), 0, 0, 0, static_cast<byte>(DIV.get() >> 8), TIMA.get(), TMA.get(), TAC.get(), 0, 0, 0, 0, 0, 0, 0, IF.get(),
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            LCDC.get(), STAT.get(), SCY.get(), SCX.get(), LY.get(), LYC.get(), DMA.get(), BGP.get(), OBP0.get(), OBP1.get(), WY.get(), WX.get(), 0, KEY1.get(), 0, VBK.get(),
            BootEnabled.get(), HDMA1.get(), HDMA2.get(), HDMA3.get(), HDMA4.get(), HDMA5.get(), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, BCPS.get(), BCPD.get(), OCPS.get(), OCPD.get(), 0, 0, 0, 0,
            SVBK.get(), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, IE.get()};

        return &ioReg;
    }

    const std::vector<byte> *Bus::getHram()
    {
        return &hram;
    }

}
