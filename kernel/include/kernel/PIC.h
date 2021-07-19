#ifndef _KERNEL_PIC_H
#define _KERNEL_PIC_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void initialize_PICs(uint8_t irq_offset);
uint8_t read_IMR(bool read_slave);
void set_OCW(uint8_t OCW_num, uint8_t OCW_val);

#endif