#include <kernel/serial_ports.h>
#include <kernel/io.h>
#include <kernel/PIC.h>

#define OCW1 0x021

#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8

#define DEFAULT_BAUD_RATE 4800

bool enable_serial_port(void) {
    uint16_t lowest_port = 0;

    lowest_port = lowest_available_port();
    if (lowest_port != 0) {
        printf("COM port found!\n");
        initialize_port(lowest_port);
        return true;
    }
    return false;
}

uint16_t lowest_available_port(void) {
    outb(COM1 + 4, 0b1000); // Enable loopback mode on COM1
    outb(COM1 + 7, 0x05); // Write a value to the scratch register
    if (inb(COM1 + 7) == 0x05) {
        outb(COM1 + 4, 0b0000); // Disable loopback mode on COM1
        return COM1; // Try to read that value back
    }

    //Repeat for COM2 if COM1 not found
    outb(COM2 + 4, 0b1000);
    outb(COM2 + 7, 0x05);
    if (inb(COM2 + 7) == 0x05) {
        return COM2; //
    }
    return 0;
}

void initialize_port(uint16_t port) {
    // Call to PIC module to unmask COM IRQ
    uint8_t current_IMR = read_IMR(false);
    if (port == COM1 || port == COM3) {
        current_IMR = current_IMR & 0xEF;
        set_OCW(1, current_IMR);
    } else if (port == COM2 || port == COM3) {
        current_IMR = current_IMR & 0xF7;
        set_OCW(1, current_IMR);
    }

    set_baud_rate(port, DEFAULT_BAUD_RATE);
    outb(port + 3, 0x17); // Set the word length to 8 bits
    outb(port + 1, 0x16); // Set the Transmitter Holding Register Empty Interrupt bit
}

void set_baud_rate(uint16_t port, uint32_t baud_rate) {
    uint16_t baud_divisor = 115200 / baud_rate;

    outb(port + 3, 0x80); // Set Divisor Latch Access Bit
    outb(port, (baud_divisor & 0xFF)); // Set Divisor Latch Low Byte
    outb(port + 1, (baud_divisor >> 8)); // Set Divisor Latch High Byte
    outb(port + 3, 0x00); // Clear Divisor Latch Access Bit
}

int serial_recieved(uint16_t port) {
    return inb(port + 5) & 1;
}

int is_transmit_empty(uint16_t port) {
    return inb(port + 5) & 0x20;
}

// TODO: Implement these with dynamic buffers so they are non-blocking.

void send_data(uint16_t port, char * buffer, int length) {
    for (int i = 0; i < length; i++) {
        send_char(port, buffer[i]);
    }
}

void send_char(uint16_t port, char char_to_send) {
    while (is_transmit_empty(port) == 0);

    outb(port, (uint8_t)char_to_send);
}

char read_data(uint16_t port) {
    while (serial_recieved(port) == 0);

    return inb(port);
}