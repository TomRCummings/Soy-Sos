
#include <kernel/descriptor_tables.h>

extern void _gdt_flush(uint32_t);
extern void _idt_set(uint32_t);

extern void _isr_wrapper(void);
extern void _com1_wrapper(void);
extern void _com2_wrapper(void);

static void init_gdt(void);
static void gdt_set_entry(int32_t, uint32_t, uint32_t, uint8_t, uint8_t);

static void init_idt(void);
static void idt_set_entry(int32_t, uint32_t, uint16_t, uint8_t, uint8_t, uint8_t, uint8_t);

gdt_entry_t gdt_entries[3];
gdt_ptr_t gdt_ptr;

__attribute__((aligned(0x10)))
static idt_entry_t idt_entries[255];
idt_ptr_t idt_ptr;

void init_descriptor_tables() {
    init_gdt();
    init_idt();
}

static void init_gdt() {
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 3) - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;

    gdt_set_entry(0,0,0,0,0);
    gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    _gdt_flush((uint32_t)&gdt_ptr);
}

static void init_idt() {
    idt_ptr.limit = (sizeof(idt_entry_t) * 255) - 1;
    idt_ptr.base = (uint32_t)&idt_entries;

    //Set IDT entries here!
    idt_set_entry(200, (uint32_t)&_isr_wrapper, 0x8, 1, 0, 0, 0xE);
    idt_set_entry(0x24, (uint32_t)&_com1_wrapper, 0x8, 1, 0, 0, 0xE);
    idt_set_entry(0x23, (uint32_t)&_com2_wrapper, 0x8, 1, 0, 0, 0xE);

    _idt_set((uint32_t)&idt_ptr);
}

static void gdt_set_entry(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granul) {
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;
    
    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity |= granul & 0xF0;
    gdt_entries[num].access = access;
}

static void idt_set_entry(int32_t num, uint32_t offset, uint16_t selector, uint8_t present_bit, uint8_t descriptor_privilege, uint8_t storage_bit, uint8_t gate_type) {
    idt_entries[num].offset_low = offset & 0xFFFF;
    idt_entries[num].selector = selector;
    idt_entries[num].zeroes = 0x0;
    idt_entries[num].attrs = gate_type;
    idt_entries[num].attrs |= storage_bit << 4;
    idt_entries[num].attrs |= descriptor_privilege << 5;
    idt_entries[num].attrs |= present_bit << 7;
    idt_entries[num].offset_high = offset  >> 16;
}