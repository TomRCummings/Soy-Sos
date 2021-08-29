#include <stdio.h>
#include <stdlib.h>

#include <kernel/multiboot.h>
#include <kernel/tty.h>
#include <kernel/descriptor_tables.h>
#include <kernel/PIC.h>
#include <kernel/serial_ports.h>

#define DEFAULT_PIC_IRQ 0x20

extern bool serial_port_enabled = false;

void kernel_main(multiboot_info_t* mbd, unsigned int magic) {

	terminal_initialize();

	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		//TODO: Implement kpanic()
	}

	if (!(mbd->flags >> 6 & 0x1)) {
		//TODO: Implement kpanic()
	}

	for (int i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t)) {
		multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*) (mbd->mmap_addr + i);
		printf("Start Addr: %X | Length: %X | Size: %X | Type: %d\n", mmmt->addr_low, mmmt->len_low, mmmt->size, mmmt->type);
	}

	init_descriptor_tables();

	printf("Descriptor tables and terminal initialized!\n");

	initialize_PICs(DEFAULT_PIC_IRQ);

	if (enable_serial_port()) {
		serial_port_enabled = true;
		printf("Serial port intialized!\n");
		serial_print(0x3F8, "Serial port initialized!\n");
	}
}