#pragma once

#include <cstdint>
#include <expected>

#include "cartridge.hpp"

namespace gameboy
{
const CartridgeInfo getCartridgeInfo(const std::vector<uint8_t> &);
size_t getRomSize(const uint8_t);
size_t getRamSize(const uint8_t);
std::expected<std::shared_ptr<Cartridge>, std::string>
getCartridge(std::string &);
} // namespace gameboy
