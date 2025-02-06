#pragma once

#include <memory>

#include "cartridge.hpp"

namespace emulator::components
{
    const CartridgeInfo getCartridgeInfo(const std::vector<byte> &);
    size_t getRomSize(const byte);
    size_t getRamSize(const byte);
    std::shared_ptr<Cartridge> getCartridge(std::string &);
}
