.global _com1_wrapper
.align 4

_com1_wrapper:
    pushal
    cld
    call com1_handler
    popal
    iret

.global _com2_wrapper
.align 4

_com2_wrapper:
    pushal
    cld
    call com2_handler
    popal
    iret