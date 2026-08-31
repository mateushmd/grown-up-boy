#include "bus.h"
#include "gb.h"

namespace emulator {
    Bus::Bus() { }

    uint8_t read_io(uint16_t address, struct EmulatorContext &context) {
        if (address == 0xff00) {
            //joypad
        } else if (address <= 0xff02) {
            //serial transfer
        } else if (address == 0xff03) {
            TODO("unknown");
        } else if (address <= 0xff07) {
            //timer and divider
        } else if (address <= 0xff0e) {
            TODO("unknown");
        } else if (address == 0xff0f) {
            //interrupts
        } else if (address <= 0xff26) {
            //audio
        } else if (address <= 0xff29) {
            TODO("unknown");
        } else if (address <= 0xff3f) {
            //wave pattern
        } else if (address <= 0xff45) {
            //lcd 
        } else if (address == 0xff46) {
            //oam dma transfer
        } else if (address <= 0xff4b) {
            //lcd
        }  else if (address == 0xff51) {
            //boot rom mapping control
        } else {
            TODO("cgb");
        } 
    }

    void write_io(uint16_t address, uint8_t value, struct EmulatorContext &context){
        if (address == 0xff00) {
            //joypad
        } else if (address <= 0xff02) {
            //serial transfer
        } else if (address == 0xff03) {
            TODO("unknown");
        } else if (address <= 0xff07) {
            //timer and divider
        } else if (address <= 0xff0e) {
            TODO("unknown");
        } else if (address == 0xff0f) {
            //interrupts
        } else if (address <= 0xff26) {
            //audio
        } else if (address <= 0xff29) {
            TODO("unknown");
        } else if (address <= 0xff3f) {
            //wave pattern
        } else if (address <= 0xff45) {
            //lcd 
        } else if (address == 0xff46) {
            //oam dma transfer
        } else if (address <= 0xff4b) {
            //lcd
        }  else if (address == 0xff51) {
            //boot rom mapping control
        } else {
            TODO("cgb");
        } 
    }

    uint8_t Bus::read(uint16_t address, struct EmulatorContext &context) {
        switch (address >> 12) {
            case 0x0: case 0x1: case 0x2: case 0x3:
                return context.cartridge.read_rom(address);

            case 0x4: case 0x5: case 0x6: case 0x7:
                // TODO: CGB: implement bank switching
                return context.cartridge.read_rom(address);
            
            case 0x8: case 0x9:
                return context.vram[address - 0x8000];

            case 0xa: case 0xb:
                return context.cartridge.read_ram(address - 0xa000);

            case 0xc: case 0xd:
                return context.wram[address - 0xc000];

            case 0xe: 
                return read(address - 0x2000, context);

            case 0xf:
                if (address <= 0xFDFF) {
                    return read(address - 0x2000, context);
                } else if (address <= 0xFE9F) {
                    return context.oam[address - 0xFE00];
                } else if (address <= 0xFEFF) {
                    return 0xFF;
                } else if (address <= 0xFF7F) {
                    return read_io(address, context);
                } else if (address <= 0xFFFE) {
                    return context.hram[address - 0xFF80];
                } else {
                    return context.ie;
                }
        }
    } 

    void Bus::write(uint16_t address, uint8_t value, struct EmulatorContext &context) {
        switch (address >> 12) {
            case 0x0: case 0x1: case 0x2: case 0x3:
            case 0x4: case 0x5: case 0x6: case 0x7:
                break;
            
            case 0x8: case 0x9:
                context.vram[address - 0x8000] = value;
                break;

            case 0xa: case 0xb:
                context.cartridge.write_ram(address - 0xa000, value);
                break;

            case 0xc: case 0xd:
                context.wram[address - 0xc000] = value;
                break;

            case 0xe: 
                write(address - 0x2000, value, context);
                break;

            case 0xf:
                if (address <= 0xFDFF) {
                    write(address - 0x2000, value, context);
                } else if (address <= 0xFE9F) {
                    context.oam[address - 0xFE00] = value;
                } else if (address >= 0xFF00 && address <= 0xFF7F) {
                    write_io(address, value, context);
                } else if (address <= 0xFFFE) {
                    context.hram[address - 0xFF80] = value;
                } else {
                    context.ie = value;
                }
        }
    }
}
