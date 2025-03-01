#pragma once

#include <string>
#include <stdexcept>
#include <vector>
#include <memory>

#include "logger.hpp"
#include "types.hpp"
#include "cartridge.hpp"
#include "timer.hpp"
#include "registers.hpp"

namespace emulator::components
{
    constexpr size_t BOOT_SIZE = 100;
    constexpr size_t VRAM_SIZE = 1024 * 8;
    constexpr size_t WRAM_SIZE = 1024 * 4;
    constexpr size_t OAM_SIZE = 160;
    constexpr size_t HRAM_SIZE = 127;
    constexpr size_t IOREG_SIZE = 127;

    constexpr byte DIV_ADDR = 0x0004;

    class Bus
    {
    private:
        bool cgbMode;

        std::vector<byte> bootRom;
        std::vector<byte> vram;
        std::vector<byte> wram;
        std::vector<byte> oam;
        std::vector<byte> hram;
        std::vector<byte> ioReg;

        Register8 JOYP, TIMA, TMA, TAC, IF, LCDC, STAT, SCY, SCX, LY, LYC, DMA,
            BGP, OBP0, OBP1, WY, WX, KEY1, VBK, BootEnabled, HDMA1, HDMA2, HDMA3,
            HDMA4, HDMA5, BCPS, BCPD, OCPS, OCPD, SVBK, IE;

        Register16 DIV;

        std::shared_ptr<Cartridge> cartridge;

        bool dmaTransfer;

    public:
        Bus(bool);

        void setBootRom(std::string);
        void setCartridge(std::shared_ptr<Cartridge>);

        byte &getCell(word);
        byte read(word);
        void write(word, const byte);
        byte readRegister(word);
        void writeRegister(word, byte);

        std::unique_ptr<Timer> getTimer();

        const std::vector<byte> *getBootRom();
        const std::vector<byte> *getCartridgeRom();
        const std::vector<byte> *getVram();
        const std::vector<byte> *getCartridgeRam();
        const std::vector<byte> *getWram();
        const std::vector<byte> *getOam();
        const std::vector<byte> *getIOReg();
        const std::vector<byte> *getHram();
    };
}