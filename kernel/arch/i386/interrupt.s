.macro ISR_NOERRCODE p
.global _isr\p
_isr\p:
    pushl $0
    pushl $\p
    jmp isr_common_stub
.endm

.macro ISR_ERRCODE pa
.global _isr\pa
_isr\pa:
    pushl $\pa
    jmp isr_common_stub
.endm

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE 8
ISR_ERRCODE 10
ISR_ERRCODE 11
ISR_ERRCODE 12
ISR_ERRCODE 13
ISR_ERRCODE 14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_ERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_ERRCODE 30


isr_common_stub:
    pushal
    
    mov %ds, %ax
    push %eax
    
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    mov %esp, %eax # this is my attempt to pass the pointer to the registers_t struct to the function
    add $68, %eax

    push %eax

    call isr_handler

    pop %eax

    pop %eax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    popal
    add $8, %esp
    iret
