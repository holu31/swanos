.global gdt_flush
.global idt_load

idt_load:
    mov 4(%esp), %eax
    lidt (%eax)
    ret

gdt_flush:
    cli

    mov 4(%esp), %eax
    lgdt (%eax)

    mov $0x10, %ax # 0x10 is the offset in the GDT to our data segment 
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    jmp $0x08, $.flush

.flush:
    ret # back to c
