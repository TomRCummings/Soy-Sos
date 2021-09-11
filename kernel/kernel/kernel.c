#include <stdio.h>
#include <stdlib.h>

#include <kernel/Multiboot.h>
#include <kernel/TTY.h>
#include <kernel/DescriptorTables.h>
#include <kernel/PIC.h>
#include <kernel/Serial.h>
#include <kernel/MemoryManager.h>

#define DEFAULT_PIC_IRQ 0x20

extern void* endOfKernel;

extern bool serial_port_enabled = false;

void kernel_main(multiboot_info_t* mbd, unsigned int magic) {

	terminal_initialize();

	uint32_t endOfKernelAddress = endOfKernel;
	printf("The end of the kernel is at: %d\n", endOfKernelAddress);

	initialize_memory_manager(endOfKernelAddress);

	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		//TODO: Implement kpanic()
	}

	if (!(mbd->flags >> 6 & 0x1)) {
		//TODO: Implement kpanic()
	}

	init_descriptor_tables();

	printf("Descriptor tables and terminal initialized!\n");

	for (int i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t)) {
		multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*) (mbd->mmap_addr + i);

		//TODO: This implementation tacks the page list onto the end of the kernel. This won't work for higher half and isn't very smart.
		if (mmmt->type == 1 && mmmt->addr_low > 1000000) {
			printf("Mem map addr: %d | Mem map length: %d\n", mmmt->addr_low, mmmt->len_low);
			//parse_memory_map(mmmt);
		}
	}

	/*
	uint32_t addrs;
	for (int i = 0; i < 5; i++) {
		addrs = (uint32_t) (allocate_page());
		printf("Address of page allocated: %d\n", addrs);
	}
	*/

	initialize_PICs(DEFAULT_PIC_IRQ);

	if (enable_serial_port()) {
		serial_port_enabled = true;
		printf("Serial port intialized!\n");
		serial_print(0x3F8, "Serial port initialized!\n");
	}
}