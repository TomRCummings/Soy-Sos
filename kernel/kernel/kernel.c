#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/descriptor_tables.h>
#include <kernel/PIC.h>
#include <kernel/serial_ports.h>

#define DEFAULT_PIC_IRQ 0x20

extern bool serial_port_enabled = false;

void kernel_main(void) {

	init_descriptor_tables();
	terminal_initialize();
	printf("Descriptor tables and terminal initialized!\n");

	initialize_PICs(DEFAULT_PIC_IRQ);

	if (enable_serial_port()) {
		serial_port_enabled = true;
		printf("Serial port intialized!\n");
		serial_print(0x3F8, "Serial port initialized!\n");
	}

	asm volatile ("int $0x3");
	asm volatile ("int $0x4");

}