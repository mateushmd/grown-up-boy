#pragma once

#include <string>
#include <stdexcept>
#include <vector>

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

        // Cartridge &cartridge;

        const byte const *activeWramBank = nullptr;
        const byte const *activeVramBank = nullptr;
        const byte const *bootRomEnabled = nullptr;

        std::vector<byte> &getMem(word &address);

    public:
        Bus(bool);

        inline byte &get(word);
        const byte read(word);
        void write(word, const byte);

        const std::vector<byte> &getVram() const;
        const std::vector<byte> &getWram() const;
        const std::vector<byte> &getOam() const;
        const std::vector<byte> &getHram() const;
        const std::vector<byte> &getIOReg() const;
    };
}