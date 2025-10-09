#pragma once

#include <memory>
#include <string>
#include <vector>

#include "cartridge.hpp"
#include "timer.hpp"

namespace gameboy
{
constexpr size_t BOOT_SIZE = 100;
constexpr size_t VRAM_SIZE = 1024 * 8;
constexpr size_t WRAM_SIZE = 1024 * 4;
constexpr size_t OAM_SIZE = 160;
constexpr size_t HRAM_SIZE = 127;
constexpr size_t IOREG_SIZE = 127;

constexpr uint8_t DIV_ADDR = 0x0004;

class Bus
{
  private:
    bool cgbMode;

    std::vector<uint8_t> bootRom;
    std::vector<uint8_t> vram;
    std::vector<uint8_t> wram;
    std::vector<uint8_t> oam;
    std::vector<uint8_t> hram;
    std::vector<uint8_t> ioReg;

    uint8_t JOYP, TIMA, TMA, TAC, IF, LCDC, STAT, SCY, SCX, LY, LYC, DMA, BGP,
        OBP0, OBP1, WY, WX, KEY1, VBK, BootEnabled, HDMA1, HDMA2, HDMA3, HDMA4,
        HDMA5, BCPS, BCPD, OCPS, OCPD, SVBK, IE;

    uint16_t DIV;

    std::shared_ptr<Cartridge> cartridge;

    bool dmaTransfer;

  public:
    Bus(bool);

    void setBootRom(std::string);
    void setCartridge(std::shared_ptr<Cartridge>);

    uint8_t &getCell(uint16_t);
    uint8_t read(uint16_t);
    void write(uint16_t, const uint8_t);
    uint8_t readRegister(uint16_t);
    void writeRegister(uint16_t, uint8_t);

    std::unique_ptr<Timer> getTimer();

    const std::vector<uint8_t> *getBootRom();
    const std::vector<uint8_t> *getCartridgeRom();
    const std::vector<uint8_t> *getVram();
    const std::vector<uint8_t> *getCartridgeRam();
    const std::vector<uint8_t> *getWram();
    const std::vector<uint8_t> *getOam();
    const std::vector<uint8_t> *getIOReg();
    const std::vector<uint8_t> *getHram();
};
} // namespace gameboy
