#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "cartridge.hpp"

namespace gameboy
{
Cartridge::Cartridge(std::shared_ptr<const CartridgeInfo> cartridgeInfo,
                     std::shared_ptr<std::vector<uint8_t>> romData)
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

uint8_t Cartridge::read(uint16_t address) { return getCell(address); }

void Cartridge::write(uint16_t address, uint8_t value)
{
    uint8_t &cellValue = getCell(address);
    cellValue = value;
}

const std::vector<uint8_t> &Cartridge::getRom() const
{
    if (!rom)
        throw std::runtime_error("Cartridge ROM is unavailable");

    return *rom;
}

const std::vector<uint8_t> &Cartridge::getRam() const
{
    if (!ram)
        throw std::runtime_error("Cartridge RAM is unavailable");

    return *ram;
}

NO_MBC::NO_MBC(std::shared_ptr<const CartridgeInfo> cartridgeInfo,
               std::shared_ptr<std::vector<uint8_t>> romData,
               std::shared_ptr<std::vector<uint8_t>> ramData)
    : Cartridge(cartridgeInfo, romData)
{
    this->ram = ramData;
}

// TODO: treat edge cases like invalid address or RAM address when there is none
uint8_t &NO_MBC::getCell(uint16_t address)
{
    if (address < 0x8000)
        return (*rom)[address];

    return (*ram)[address];
}
#pragma endregion

#pragma region MBC1
MBC1::MBC1(std::shared_ptr<const CartridgeInfo> cartridgeInfo,
           std::shared_ptr<std::vector<uint8_t>> romData,
           std::shared_ptr<std::vector<uint8_t>> ramData)
    : Cartridge(cartridgeInfo, romData)
{
    this->ram = ramData;
}

uint8_t &MBC1::getCell(uint16_t address)
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

        uint8_t romBank = activeRomBank;
        if (romBank == 0)
            romBank = 1;

        return (*rom)[address + (16 * 1024 * romBank)];
    }

    uint8_t ramBank = bankingMode ? activeRamBank : 0;

    return (*ram)[address + (8 * 1024 * ramBank)];
}

uint8_t MBC1::read(uint16_t address)
{
    if (!ramEnable && address > 0x9FFF && address < 0xC000)
        return 0xFF;

    return Cartridge::read(address);
}

void MBC1::write(uint16_t address, uint8_t value)
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
            uint8_t bitsRequired = (info->romSize < 4) ? info->romSize + 2 : 5;
            uint8_t mask = (1 << bitsRequired) - 1;
            uint8_t romBank = value & mask;

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
} // namespace gameboy
