#include <string>
#include <vector>
#include <memory>

#include "cartridge_factory.hpp"
#include "cartridge.hpp"
#include "file.hpp"
#include "logger.hpp"

namespace emulator::components
{
    // NOTE: this is bad but only runs once during runtime, change if you are a perfectionist
    const CartridgeInfo getCartridgeInfo(const std::vector<byte> &data)
    {
        CartridgeInfo info;

        info.type = data[0x0147];

        char t[TITLE_SIZE];
        for (byte i = 0; i < TITLE_SIZE; i++)
            t[i] = data[0x0134 + i];
        info.title = std::string(t);

        char lCode[] = {static_cast<char>(data[0x0144]),
                        static_cast<char>(data[0x0145])};
        info.licenseeCode = std::string(lCode);

        info.sgbFlag = data[0x0146];
        info.romSize = data[0x0148];
        info.ramSize = data[0x0149];
        info.destinationCode = data[0x014A];
        info.versionNumber = data[0x014C];
        info.headerCheckSum = data[0x014D];
        info.globalChecksum = (static_cast<word>(data[0x014E]) << 8) | data[0x014F];

        return info;
    };

    size_t getRomSize(const byte code)
    {
        size_t size = 32 * 1024;

        // TODO: threat edge case where code is wrong
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

        // TODO: threat edge case where code is wrong
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

    // TODO: check if games binaries have the same size as memories
    std::shared_ptr<Cartridge> getCartridge(std::string &file)
    {
        std::vector<byte> rom;
        util::file::loadFile(file, rom);

        const CartridgeInfo info = getCartridgeInfo(rom);

        size_t romSize = getRomSize(info.romSize);

        if (rom.size() != romSize)
        {
            logger::warning << "The binaries size is " << romSize - rom.size()
                            << " bytes different from the size specified in the"
                            << " cartridge header. The game can present problems"
                            << " while running." << std::endl;
        }

        std::vector<byte> ram(getRamSize(info.ramSize));

        switch (info.type)
        {
        case 0x00:
        case 0x08:
        case 0x09:
        {
            return std::make_shared<NO_MBC>(std::make_shared<const CartridgeInfo>(info),
                                            std::make_shared<std::vector<byte>>(rom),
                                            std::make_shared<std::vector<byte>>(ram));
        }
        case 0x01:
        case 0x02:
        case 0x03:
        {
            return std::make_shared<MBC1>(std::make_shared<const CartridgeInfo>(info),
                                          std::make_shared<std::vector<byte>>(rom),
                                          std::make_shared<std::vector<byte>>(ram));
        }
        case 0x05:
        case 0x06:
        {
        }
        case 0x0F:
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x13:
        {
        }
        }

        return nullptr;
    }
}