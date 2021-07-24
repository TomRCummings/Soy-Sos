#include <stdint.h>

typedef struct registers {
    uint32_t ds;
    uint32_t edi, esi, ebp, use_less_value, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, esp, ss;
} registers_t;