#include "io_dispatcher.h"

namespace emulator {
    uint8_t IoDispatcher::read(const uint16_t address) {
        if (address == 0) {
            return joypad.read();
        } else if (address >= 0x04 && address <= 0x07) {
            return timer.read(address - 0x04);
        }else if (address == 0x0f) {
            return interrupts.read();
        } else if (address >= 0x10 && address <= 0x26) {
            return audio.read(address - 0x10);
        } else if (address >= 0x30 && address <= 0x3f) {
            return audio.read(address - 0x30);
        } else if (address == 0x46) {
            return oam_dma_transfer; 
        } else if (address <= 0x4b) {
            return lcd.read(address - 0x40);
        } else if (address == 0x50) {
            return boot_rom_mapping_control;
        }
        
        TODO("empty region");
    }

    void IoDispatcher::write(const uint16_t address, const uint8_t value) {
        if (address == 0) {
            joypad.write(value);
        } else if (address >= 0x04 && address <= 0x07) {
            timer.write(address - 0x04, value);
        }else if (address == 0x0f) {
            interrupts.write(value);
        } else if (address >= 0x10 && address <= 0x26) {
            audio.write(address - 0x10, value);
        } else if (address >= 0x30 && address <= 0x3f) {
            audio.write(address - 0x30, value);
        } else if (address == 0x46) {
            oam_dma_transfer = value; 
        } else if (address <= 0x4b) {
            lcd.write(address - 0x40, value);
        } else if (address == 0x50) {
            boot_rom_mapping_control = value;
        }
        
        TODO("empty region");
    }
}
