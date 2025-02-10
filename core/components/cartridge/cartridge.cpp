#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

#include "cartridge.hpp"
#include "file.hpp"
#include "logger.hpp"

namespace emulator::components
{
    Cartridge::Cartridge(std::shared_ptr<const CartridgeInfo> cartridgeInfo,
                         std::shared_ptr<std::vector<byte>> romData)
        : info(cartridgeInfo), rom(romData)
    {
    }

    std::string Cartridge::toString()
    {

        std::string str = "Type: " + std::to_string(info->type) + "\n";
        str += "Title: " + info->title + "\n";
        str += "Licensee code: " + info->licenseeCode + "\n";
        str += "SGB flag: " + std::to_string(info->sgbFlag) + "\n";
        str += "ROM size: " + std::to_string(info->romSize) + "\n";
        str += "RAM size: " + std::to_string(info->ramSize) + "\n";
        str += "Destination code: " + std::to_string(info->destinationCode) + "\n";
        str += "Version number: " + std::to_string(info->versionNumber) + "\n";
        str += "Header cheksum: " + std::to_string(info->headerCheckSum) + "\n";
        str += "Global checksum: " + std::to_string(info->globalChecksum);
        return str;
    }

    byte Cartridge::read(word address)
    {
        return getCell(address);
    }

    void Cartridge::write(word address, byte value)
    {
        byte &cellValue = getCell(address);
        cellValue = value;
    }

    const std::vector<byte> &Cartridge::getRom() const
    {
        if (!rom)
            throw std::runtime_error("Cartridge ROM is unavailable");

        return *rom;
    }

    const std::vector<byte> &Cartridge::getRam() const
    {
        if (!ram)
            throw std::runtime_error("Cartridge RAM is unavailable");

        return *ram;
    }

#pragma region NO_MBC
    NO_MBC::NO_MBC(std::shared_ptr<const CartridgeInfo> cartridgeInfo,
                   std::shared_ptr<std::vector<byte>> romData,
                   std::shared_ptr<std::vector<byte>> ramData)
        : Cartridge(cartridgeInfo, romData)
    {
        this->ram = ramData;
    }

    // TODO: treat edge cases like invalid address or RAM address when there is none
    byte &NO_MBC::getCell(word address)
    {
        if (address < 0x8000)
            return (*rom)[address];

        return (*ram)[address];
    }
#pragma endregion

#pragma region MBC1
    MBC1::MBC1(std::shared_ptr<const CartridgeInfo> cartridgeInfo,
               std::shared_ptr<std::vector<byte>> romData,
               std::shared_ptr<std::vector<byte>> ramData)
        : Cartridge(cartridgeInfo, romData)
    {
        this->ram = ramData;
    }

    byte &MBC1::getCell(word address)
    {
        if (address < 0x4000)
        {
            if (bankingMode == 1)
                return (*rom)[address + (16 * 1024 * (activeRamBank << 5))];

            return (*rom)[address];
        }
        if (address < 0x8000)
        {
            address -= 0x4000;

            byte romBank = activeRomBank;
            if (romBank == 0)
                romBank = 1;

            return (*rom)[address + (16 * 1024 * romBank)];
        }

        byte ramBank = bankingMode ? activeRamBank : 0;

        return (*ram)[address + (8 * 1024 * ramBank)];
    }

    byte MBC1::read(word address)
    {
        if (!ramEnable && address > 0x9FFF && address < 0xC000)
            return 0xFF;

        return Cartridge::read(address);
    }

    void MBC1::write(word address, byte value)
    {
        if (address < 0x2000)
            ramEnable = (value == 0x0A);
        else if (address < 0x4000)
        {
            if (value == 0 || value == 0xE1)
                activeRomBank = 1;
            else if (value < 0x20)
                activeRomBank = value;
            else
            {
                byte bitsRequired = (info->romSize < 4) ? info->romSize + 2 : 5;
                byte mask = (1 << bitsRequired) - 1;
                byte romBank = value & mask;

                if (info->romSize > 3)
                    romBank |= activeRamBank << 5;

                activeRomBank = romBank;
            }
        }
        else if (address < 0x6000)
            activeRamBank = value & 0b11;
        else if (address < 0x8000)
            bankingMode = (value & 1) == 1;

        Cartridge::write(address, value);
    }

#pragma endregion
}