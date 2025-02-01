#pragma once

#include <vector>
#include <string>

#include "types.hpp"

namespace emulator::components
{
    struct CartridgeInfo
    {
        byte type;
        std::string title;
        char licenseeCode[2];
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
        const CartridgeInfo &info;

        std::vector<byte> rom;
        std::vector<byte> ram;

    public:
        Cartridge(const CartridgeInfo &, const std::vector<byte>, int);
        Cartridge(const CartridgeInfo &, const std::vector<byte>);

        virtual byte read(const word) const = 0;
        virtual void write(const word, const byte) = 0;
        const CartridgeInfo getInfo();
    };

    class NO_MBC : public Cartridge
    {
    public:
        NO_MBC(const CartridgeInfo &, const std::vector<byte>);

        byte read(const word) const override;
        void write(const word, const byte) override;
    };

    class MBC1 : public Cartridge
    {
    public:
        MBC1(const CartridgeInfo &, const std::vector<byte>);

        byte read(const word) const override;
        void write(const word, const byte) override;
    };

    class MBC2 : public Cartridge
    {
    public:
        MBC2(const CartridgeInfo &, const std::vector<byte>);

        byte read(const word) const override;
        void write(const word, const byte) override;
    };

    class MBC3 : public Cartridge
    {
    public:
        MBC3(const CartridgeInfo &, const std::vector<byte>);

        byte read(const word) const override;
        void write(const word, const byte) override;
    };

    const CartridgeInfo &getCartridgeInfo(const std::vector<byte> &);
    size_t getRomSize(const byte);
    size_t getRamSize(const byte);
    Cartridge &getCartridge(std::string &);
}