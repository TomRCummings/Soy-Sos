#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/descriptor_tables.h>
#include <kernel/PIC.h>
#include <kernel/serial_ports.h>

#define DEFAULT_PIC_IRQ 0x20

void kernel_main(void) {
	init_descriptor_tables();
	terminal_initialize();
	printf("Descriptor tables and terminal initialized!\n");

	initialize_PICs(DEFAULT_PIC_IRQ);

	if (enable_serial_port()) {
		send_data(0x3F8, "Hello World!\n", 13);
	}
}