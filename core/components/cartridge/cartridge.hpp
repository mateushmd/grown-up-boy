#pragma once

#include <vector>
#include <string>
#include <memory>

#include "types.hpp"

namespace emulator::components
{
    constexpr byte TITLE_SIZE = 11;

    struct CartridgeInfo
    {
        byte type;
        std::string title;
        std::string licenseeCode;
        byte sgbFlag;
        byte romSize;
        byte ramSize;
        byte destinationCode;
        byte versionNumber;
        byte headerCheckSum;
        word globalChecksum;
    };

    class Cartridge
    {
    protected:
        std::shared_ptr<const CartridgeInfo> info = nullptr;

        std::shared_ptr<std::vector<byte>> rom = nullptr;
        std::shared_ptr<std::vector<byte>> ram = nullptr;

        virtual byte &getCell(word) = 0;

    public:
        Cartridge(std::shared_ptr<const CartridgeInfo>, std::shared_ptr<std::vector<byte>>);

        virtual const byte read(word);
        virtual void write(word, byte);
        std::string toString();
    };

    class NO_MBC : public Cartridge
    {
    protected:
        byte &getCell(word) override;

    public:
        NO_MBC(std::shared_ptr<const CartridgeInfo>, std::shared_ptr<std::vector<byte>>, std::shared_ptr<std::vector<byte>>);
    };

    class MBC1 : public Cartridge
    {
    private:
        bool ramEnable;
        byte activeRomBank;
        byte secondaryActiveRomBank;
        byte activeRamBank;
        bool bankingMode;

    protected:
        byte &getCell(word) override;

    public:
        MBC1(std::shared_ptr<const CartridgeInfo>, std::shared_ptr<std::vector<byte>>, std::shared_ptr<std::vector<byte>>);

        const byte read(word);
        void write(word, byte) override;
    };

    /*
    class MBC2 : public Cartridge
    {
    public:
        byte &getCell(word &) override;
    };

    class MBC3 : public Cartridge
    {
    public:
        byte &getCell(word &) override;
    };
    */
}