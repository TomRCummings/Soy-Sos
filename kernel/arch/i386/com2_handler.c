#include <kernel/io.h>

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void com2_handler() {
    uint8_t com2_status = inb(0x2F8);
    //printf("COM2 handler triggered.\n");

    // TODO: Check serial buffer on file system. If data present
    // and the "transmitter holding register empty" is set, pop
    // a byte of data off the buffer.

    outb(0x20, 0x20); // Acknowledge and end interrupt
}
