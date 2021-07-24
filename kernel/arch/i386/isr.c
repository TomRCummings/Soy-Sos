#include <kernel/isr.h>

#include <stdio.h>
#include <string.h>

void isr_handler(registers_t *regs) {


    printf("recieved interrupt: ");
    printf_int(&regs->int_no);
    printf("\n");
}