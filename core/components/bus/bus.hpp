#pragma once

#include <string>
#include <stdexcept>
#include <vector>
#include <memory>

#include "logger.hpp"
#include "types.hpp"
#include "cartridge.hpp"

namespace emulator::components
{
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

        std::shared_ptr<Cartridge> cartridge;

        const byte *activeWramBank = nullptr;
        const byte *activeVramBank = nullptr;
        const byte *bootRomEnabled = nullptr;

    public:
        Bus(bool);

        void setCartridge(std::shared_ptr<Cartridge>);

        byte &getCell(word);
        byte read(word);
        void write(word, const byte);

        const std::vector<byte> &getBootRom() const;
        const std::vector<byte> &getCartridgeRom() const;
        const std::vector<byte> &getVram() const;
        const std::vector<byte> &getCartridgeRam() const;
        const std::vector<byte> &getWram() const;
        const std::vector<byte> &getOam() const;
        const std::vector<byte> &getIOReg() const;
        const std::vector<byte> &getHram() const;
    };
}