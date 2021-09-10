#include <kernel/IO.h>

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void com1_handler() {
    uint8_t com1_status = inb(0x3F8);

    // TODO: Check serial buffer on file system. If data present
    // and the "transmitter holding register empty" is set, pop
    // a byte of data off the buffer.

    outb(0x20, 0x20); // Acknowledge and end interrupt
}
