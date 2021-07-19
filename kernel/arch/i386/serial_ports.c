#include <kernel/serial_ports.h>
#include <kernel/io.h>
#include <kernel/PIC.h>

#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

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

static bool send_data(uint16_t port, const char * data, size_t length) {
    for (size_t i = 0; i < length; i++) {
        if (data[i] == -1) {
            return false;
        } else {
            send_char(port, data[i]);
        }
    }
    return true;
}

int serial_print(uint16_t port, const char* restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);

	int written = 0;

	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;

		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!send_data(port, format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format++;

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!send_data(port, &c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!send_data(port, str, len))
				return -1;
			written += len;
		} else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!send_data(port, format, len))
				return -1;
			written += len;
			format += len;
		}
	}

	va_end(parameters);
	return written;
}

void send_char(uint16_t port, char char_to_send) {
    while (is_transmit_empty(port) == 0);

    outb(port, (uint8_t)char_to_send);
}

char read_data(uint16_t port) {
    while (serial_recieved(port) == 0);

    return inb(port);
}