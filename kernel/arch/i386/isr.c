#include <kernel/isr.h>
#include <kernel/in_out.h>

#include <stdio.h>
#include <string.h>

void isr_handler(registers_t regs) {
    printf("recieved interrupt:");
    printf("%d", regs.int_no);
    printf("\n");
}