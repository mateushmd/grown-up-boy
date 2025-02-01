#include <string>
#include <vector>

#include "cartridge.hpp"
#include "file.hpp"

namespace emulator::components
{
    const CartridgeInfo &getCartridgeInfo(const std::vector<byte> &data)
    {
        CartridgeInfo info;

        info.type = data[0x0147];
        info.title = std::string(data.at(0x0134), data.at(0x013E));
        info.licenseeCode[0] = static_cast<char>(data[0x0144]);
        info.licenseeCode[1] = static_cast<char>(data[0x0145]);
        info.sgbFlag = data[0x0146];
        info.romSize = data[0x0148];
        info.ramSize = data[0x0149];
        info.destinationCode = data[0x014A];
        info.versionNumber = data[0x014C];
        info.headerCheckSum = data[0x014D];
        info.globalChecksum = (static_cast<word>(data[0x014E]) << 8) | data[0x014F];

        return info;
    };

    Cartridge &getCartridge(std::string &file)
    {
        std::vector<byte> fileData = util::file::loadFile(file, 32 * 1024);

        const CartridgeInfo &cartridgeInfo = getCartridgeInfo(fileData);

        switch (cartridgeInfo.type)
        {
        case 0x00:
        case 0x08 ... 0x09:
        {
            NO_MBC cartridge(cartridgeInfo, fileData);
        }
        case 0x01 ... 0x03:
        {
        }
        case 0x05 ... 0x06:
        {
        }
        case 0x0F ... 0x13:
        {
        }
        }
    }

    size_t getRomSize(const byte code)
    {
        size_t size = 32 * 1024;

        if (code < 9)
            size *= (1 << code);
        else if (code == 52)
            size = 1.1 * 1024 * 1024;
        else if (code == 53)
            size = 1.2 * 1024 * 1024;
        else if (code == 54)
            size = 1.5 * 1024 * 1024;

        return size;
    }

    size_t getRamSize(const byte code)
    {
        size_t size = 0;

        switch (code)
        {
        case 0x2:
            size = 1024 * 8;
            break;
        case 0x3:
            size = 1024 * 32;
            break;
        case 4:
            size = 1024 * 128;
            break;
        case 5:
            size = 1024 * 64;
            break;
        }

        return size;
    }

    Cartridge::Cartridge(const CartridgeInfo &cartridgeInfo,
                         const std::vector<byte> romData,
                         int ramSize)
        : info(cartridgeInfo), rom(std::move(romData)), ram(ramSize)
    {
    }

    Cartridge::Cartridge(const CartridgeInfo &cartridgeInfo,
                         const std::vector<byte> romData)
        : info(cartridgeInfo), rom(std::move(romData))
    {
        ram = std::vector<byte>(getRamSize(cartridgeInfo.ramSize));
    }

    const CartridgeInfo Cartridge::getInfo()
    {
        return info;
    }

    MBC1::MBC1(const CartridgeInfo &cartridgeInfo,
               const std::vector<byte> romData)
        : Cartridge(cartridgeInfo, romData)
    {
    }

    MBC2::MBC2(const CartridgeInfo &cartridgeInfo,
               const std::vector<byte> romData)
        : Cartridge(cartridgeInfo, romData, 256)
    {
    }
}