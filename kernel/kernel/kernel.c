#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/descriptor_tables.h>

extern void _test_idt(void);

void kernel_main(void) {
	terminal_initialize();
	init_descriptor_tables();

	printf("Descriptor tables and terminal initialized!\n");

}
