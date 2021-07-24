
#include <kernel/descriptor_tables.h>

extern void _gdt_flush(uint32_t);
extern void _idt_set(uint32_t);

extern void _isr0(void);
extern void _isr1(void);
extern void _isr2(void);
extern void _isr3(void);
extern void _isr4(void);
extern void _isr5(void);
extern void _isr6(void);
extern void _isr7(void);
extern void _isr8(void);
extern void _isr10(void);
extern void _isr11(void);
extern void _isr12(void);
extern void _isr13(void);
extern void _isr14(void);
extern void _isr15(void);
extern void _isr16(void);
extern void _isr17(void);
extern void _isr18(void);
extern void _isr19(void);
extern void _isr20(void);
extern void _isr30(void);
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

    //Currently uses a flat memory model
    gdt_set_entry(0,0,0,0,0);
    gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    _gdt_flush((uint32_t)&gdt_ptr);
}

static void init_idt() {
    idt_ptr.limit = (sizeof(idt_entry_t) * 255) - 1;
    idt_ptr.base = (uint32_t)&idt_entries;

    //Set IDT entries here!
    //Exceptions
    idt_set_entry(0x0, (uint32_t)&_isr0, 0x8, 1, 0, 0, 0xF);
    idt_set_entry(0x1, (uint32_t)&_isr1, 0x8, 1, 0, 0, 0xF);
    idt_set_entry(0x2, (uint32_t)&_isr2, 0x8, 1, 0, 0, 0xF);
    idt_set_entry(0x3, (uint32_t)&_isr3, 0x8, 1, 0, 0, 0xF);
    idt_set_entry(0x4, (uint32_t)&_isr4, 0x8, 1, 0, 0, 0xF);
    idt_set_entry(0x5, (uint32_t)&_isr5, 0x8, 1, 0, 0, 0xF);
    idt_set_entry(0x6, (uint32_t)&_isr6, 0x8, 1, 0, 0, 0xF);
    idt_set_entry(0x7, (uint32_t)&_isr7, 0x8, 1, 0, 0, 0xF);
    idt_set_entry(0x8, (uint32_t)&_isr8, 0x8, 1, 0, 0, 0xF);
    idt_set_entry(0x10, (uint32_t)&_isr10, 0x8, 1, 0, 0, 0xF);
    idt_set_entry(0x11, (uint32_t)&_isr11, 0x8, 1, 0, 0, 0xF);
    idt_set_entry(0x12, (uint32_t)&_isr12, 0x8, 1, 0, 0, 0xF);
    idt_set_entry(0x13, (uint32_t)&_isr13, 0x8, 1, 0, 0, 0xF);
    idt_set_entry(0x14, (uint32_t)&_isr14, 0x8, 1, 0, 0, 0xF);
    idt_set_entry(0x15, (uint32_t)&_isr15, 0x8, 1, 0, 0, 0xF);
    idt_set_entry(0x16, (uint32_t)&_isr16, 0x8, 1, 0, 0, 0xF);
    idt_set_entry(0x17, (uint32_t)&_isr17, 0x8, 1, 0, 0, 0xF);
    idt_set_entry(0x18, (uint32_t)&_isr18, 0x8, 1, 0, 0, 0xF);
    idt_set_entry(0x19, (uint32_t)&_isr19, 0x8, 1, 0, 0, 0xF);
    idt_set_entry(0x20, (uint32_t)&_isr20, 0x8, 1, 0, 0, 0xF);
    idt_set_entry(0x30, (uint32_t)&_isr30, 0x8, 1, 0, 0, 0xF);
    //IRQs
    idt_set_entry(0x23, (uint32_t)&_com2_wrapper, 0x8, 1, 0, 0, 0xE);
    idt_set_entry(0x24, (uint32_t)&_com1_wrapper, 0x8, 1, 0, 0, 0xE);


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