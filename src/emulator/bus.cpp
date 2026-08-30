#include "bus.h"
#include "gb.h"

namespace emulator {
    Bus::Bus() { }

    uint8_t Bus::read(uint16_t address, struct EmulatorContext &context) {
        if (address < 0x4000) { // ROM bank 0
            return context.cartridge.read_rom(address);
        } else if (address < 0x8000) { // ROM bank 1-n
            // CGB: implement bank switching
            return context.cartridge.read_rom(address);
        } else if (address < 0xa000) { // vram
            return context.vram[address - 0x8000];
        } else if (address < 0xc000) { // eram
            return context.cartridge.read_ram(address - 0xa000);
        } else if (address < 0xd000) { // wram bank 0
            return context.wram[address - 0xc000];
        } else if (address < 0xe000) { // wram bank 1-n
            // CGB: implement bank switching
            return context.wram[address - 0xd000];
        } else if (address < 0xfe00) {
            TODO("implement echo RAM");
        } else if (address < 0xfea0) {
            return context.oam[address - 0xfe00];
        } else if (address < 0xff00) {
            TODO("implement not usable range");
        } else if (address < 0xff01) {
            //joypad
        } else if (address < 0xff03) {
            //serial transfer
        } else if (address < 0xff04) {
            TODO("unknown");
        } else if (address < 0xff08) {
            //timer and divider
        } else if (address < 0xff0f) {
            TODO("unknown");
        } else if (address < 0xff10) {
            //interrupts
        } else if (address < 0xff27) {
            //audio
        } else if (address < 0xff30) {
            TODO("unknown");
        } else if (address < 0xff40) {
            //wave pattern
        } else if (address < 0xff46) {
            //lcd 
        } else if (address < 0xff47) {
            //oam dma transfer
        } else if (address < 0xff4c) {
            //lcd
        } else if (address < 0xff50) {
            TODO("cgb");
        } else if (address < 0xff51) {
            //boot rom mapping control
        } else if (address < 0xff80) {
            TODO("cgb");
        } else if (address < 0xffff) {
            return context.hram[address - 0xff80];
        } else {
            return context.ie;
        }
    } 

    void Bus::write(uint16_t address, uint8_t value, struct EmulatorContext &context) {
        if ( address >= 0x8000 && address < 0xa000) { // vram
            context.vram[address - 0x8000] = value;
        } else if (address < 0xc000) { // eram
            context.cartridge.write_ram(address - 0xa000, value);
        } else if (address < 0xd000) { // wram bank 0
            context.wram[address - 0xc000] = value;
        } else if (address < 0xe000) { // wram bank 1-n
            // TODO: CGB: implement bank switching
            context.wram[address - 0xd000] = value;
        } else if (address < 0xfe00) {
            TODO("implement echo RAM");
        } else if (address < 0xfea0) {
            context.oam[address - 0xfe00] = value;
        } else if (address < 0xff00) {
            TODO("implement not usable range");
        } else if (address < 0xff01) {
            //joypad
        } else if (address < 0xff03) {
            //serial transfer
        } else if (address < 0xff04) {
            TODO("unknown");
        } else if (address < 0xff08) {
            //timer and divider
        } else if (address < 0xff0f) {
            TODO("unknown");
        } else if (address < 0xff10) {
            //interrupts
        } else if (address < 0xff27) {
            //audio
        } else if (address < 0xff30) {
            TODO("unknown");
        } else if (address < 0xff40) {
            //wave pattern
        } else if (address < 0xff46) {
            //lcd 
        } else if (address < 0xff47) {
            //oam dma transfer
        } else if (address < 0xff4c) {
            //lcd
        } else if (address < 0xff50) {
            TODO("cgb");
        } else if (address < 0xff51) {
            //boot rom mapping control
        } else if (address < 0xff80) {
            TODO("cgb");
        } else if (address < 0xffff) {
            context.hram[address - 0xff80] = value;
        } else {
            context.ie = value;
        }
    }
}
