#include <kernel/PIC.h>

#include <kernel/in_out.h>

#define PIC_1_ADDRESS 0x20
#define PIC_2_ADDRESS 0xA0

void initialize_PICs(uint8_t irq_offset) {
    outb((uint32_t)PIC_1_ADDRESS, 0x11); // ICW1: bit 0 and bit 4 set
    outb((uint32_t)PIC_2_ADDRESS, 0x11);
    io_wait();

    outb((uint32_t)PIC_1_ADDRESS + 1, irq_offset); // ICW2: Map IRQ to offset value
    outb((uint32_t)PIC_2_ADDRESS + 1, irq_offset + 8);
    io_wait();

    outb((uint32_t)PIC_1_ADDRESS + 1, 0x4); // ICW3: Tell PIC master which IRQ line the slave is using
    outb((uint32_t)PIC_2_ADDRESS + 1, 0x2);
    io_wait();

    outb((uint32_t)PIC_1_ADDRESS + 1, 0x1); // ICW4: Enable 80x86 mode
    outb((uint32_t)PIC_1_ADDRESS + 1, 0x1);
}

uint8_t read_IMR(bool read_slave) {
    if (read_slave) {
        return inb(PIC_2_ADDRESS + 1);
    }
    return inb(PIC_1_ADDRESS + 1);
}

void set_OCW(uint8_t OCW_num, uint8_t OCW_val) {
    outb(PIC_1_ADDRESS + (2 - OCW_num), OCW_val);
}