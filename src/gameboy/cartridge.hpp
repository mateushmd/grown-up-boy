#pragma once

#include <memory>
#include <string>
#include <vector>

namespace gameboy
{
constexpr uint8_t TITLE_SIZE = 11;

struct CartridgeInfo
{
    uint8_t type;
    std::string title;
    std::string licenseeCode;
    uint8_t sgbFlag;
    uint8_t romSize;
    uint8_t ramSize;
    uint8_t destinationCode;
    uint8_t versionNumber;
    uint8_t headerCheckSum;
    uint16_t globalChecksum;
};

class Cartridge
{
  protected:
    std::shared_ptr<const CartridgeInfo> info = nullptr;

    std::shared_ptr<std::vector<uint8_t>> rom = nullptr;
    std::shared_ptr<std::vector<uint8_t>> ram = nullptr;

    virtual uint8_t &getCell(uint16_t) = 0;

  public:
    Cartridge(std::shared_ptr<const CartridgeInfo>,
              std::shared_ptr<std::vector<uint8_t>>);

    virtual uint8_t read(uint16_t);
    virtual void write(uint16_t, uint8_t);
    std::string toString();

    const std::vector<uint8_t> &getRom() const;
    const std::vector<uint8_t> &getRam() const;
};

class NO_MBC : public Cartridge
{
  protected:
    uint8_t &getCell(uint16_t) override;

  public:
    NO_MBC(std::shared_ptr<const CartridgeInfo>,
           std::shared_ptr<std::vector<uint8_t>>,
           std::shared_ptr<std::vector<uint8_t>>);
};

class MBC1 : public Cartridge
{
  private:
    bool ramEnable;
    uint8_t activeRomBank;
    uint8_t secondaryActiveRomBank;
    uint8_t activeRamBank;
    bool bankingMode;

  protected:
    uint8_t &getCell(uint16_t) override;

  public:
    MBC1(std::shared_ptr<const CartridgeInfo>,
         std::shared_ptr<std::vector<uint8_t>>,
         std::shared_ptr<std::vector<uint8_t>>);

    uint8_t read(uint16_t);
    void write(uint16_t, uint8_t) override;
};

/*
class MBC2 : public Cartridge
{
public:
    uint8_t &getCell(uint16_t &) override;
};

class MBC3 : public Cartridge
{
public:
    uint8_t &getCell(uint16_t &) override;
};
*/
} // namespace gameboy
