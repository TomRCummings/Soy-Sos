.section .init
.global _init
.type _init, @function
_init:
	push %ebp
	movl %esp, %ebp
	/* gcc will nicely put the contents of crtbegin.o's .init section here. */

.section .fini
.global _fini
.type _fini, @function
_fini:
	push %ebp
	movl %esp, %ebp
	/* gcc will nicely put the contents of crtbegin.o's .fini section here. */

.global _gdt_flush
_gdt_flush:
	movl 4(%esp), %eax
	lgdt (%eax)

	movl $0x10, %eax
	movl %eax, %ds
	movl %eax, %es
	movl %eax, %fs
	movl %eax, %gs
	movl %eax, %ss
	jmp $0x08,$.flush
.flush:
	ret

.global _idt_set
_idt_set:
	movl 4(%esp), %eax
	lidt (%eax)
	sti
	ret