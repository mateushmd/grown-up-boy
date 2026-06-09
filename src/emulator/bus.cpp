#include <cstdint>
#include <expected>
#include <span>
#include <tuple>
#include "defs.h"
#include "bus.h"

namespace emulator {
    std::expected<uint8_t, GameBoyError> Bus::read(uint16_t address) {
        if (address < 0x4000) { // ROM bank 0
            return cartridge.read_rom(address);
        } else if (address < 0x8000) { // ROM bank 1-n
            // CGB: implement bank switching
            return cartridge.read_rom(address);
        } else if (address < 0xa000) { // vram
            return vram[address - 0x8000];
        } else if (address < 0xc000) { // eram
            return cartridge.read_ram(address - 0xa000);
        } else if (address < 0xd000) { // wram bank 0
            return wram[address - 0xc000];
        } else if (address < 0xe000) { // wram bank 1-n
            // CGB: implement bank switching
            return wram[address - 0xd000];
        } else if (address < 0xfe00) {
            // TODO: implement echo RAM
            return std::unexpected(GameBoyError::unimplemented);
        } else if (address < 0xfea0) {
            return oam[address - 0xfe00];
        } else if (address < 0xff00) {
            // TODO: implement not usable range
            return std::unexpected(GameBoyError::unimplemented);
        } else if (address < 0xff80) {
            return read_io(address);
        } else if (address < 0xffff) {
            return hram[address - 0xff80];
        } else {
            return ie;
        }
    } 

    std::expected<void, GameBoyError> Bus::write(
        uint16_t address, uint8_t value
    ) {
        if (address < 0x8000) { // ROM bank 0-n
            return {};
        } else if (address < 0xa000) { // vram
            vram[address - 0x8000] = value;
            return {};
        } else if (address < 0xc000) { // eram
            return cartridge.write_ram(address - 0xa000, value);
        } else if (address < 0xd000) { // wram bank 0
            wram[address - 0xc000] = value;
            return {};
        } else if (address < 0xe000) { // wram bank 1-n
            // CGB: implement bank switching
            wram[address - 0xd000] = value;
            return {};
        } else if (address < 0xfe00) {
            // TODO: implement echo RAM
            return std::unexpected(GameBoyError::unimplemented);
        } else if (address < 0xfea0) {
            oam[address - 0xfe00] = value;
            return {};
        } else if (address < 0xff00) {
            // TODO: implement not usable range
            return std::unexpected(GameBoyError::unimplemented);
        } else if (address < 0xff80) {
            write_io(address, value);
            return {};
        } else if (address < 0xffff) {
            hram[address - 0xff80] = value;
            return {};
        } else {
            ie = value;
            return {};
        }
    }
}
