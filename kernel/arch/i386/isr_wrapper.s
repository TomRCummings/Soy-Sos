.global _isr_wrapper
.align 4

_isr_wrapper:
    pushal
    cld
    call interrupt_handler
    popal
    iret
