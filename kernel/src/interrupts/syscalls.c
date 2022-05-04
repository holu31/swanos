#include <kernel.h>


void syscall_init() {
    register_interrupt_handler(SYSCALL_IDT_INDEX, &syscall_handler);
    qemu_printf("SYSCALL INIT\n");
}


void syscall_handler(struct regs *r) {
    switch (r->eax) {
        case SC_CODE_puts:
            tty_printf("%s", (char*) ((uint32_t*)(r->ebx)));
            r->edx = (uint32_t)1;
            break;
        case SC_CODE_getscancode:
            r->edx = (uint32_t)keyboard_getscancode();
            break;
        case SC_CODE_getchar:
            r->edx = (uint32_t)keyboard_getchar();
            break;
        case SC_CODE_gets:
            r->edx = (uint32_t)keyboard_gets();
            break;
        case SC_CODE_malloc:
            r->edx = (uint32_t)kheap_malloc((int)(r->ebx));
            break;
        case SC_CODE_free:
            kheap_free((void*)(r->ebx));
            r->edx = (uint32_t)1;
            break;
        case SC_CODE_putpixel:
            set_pixel((int) ((uint32_t*) (r->ebx)), (int) ((uint32_t*) (r->ecx)), (uint32_t)((uint32_t*) (r->edx)));
            r->edx = (uint32_t)1;
            break;
        case SC_CODE_drawline:
            set_line((int) ((uint32_t*) (r->ebx)), (int) ((uint32_t*) (r->ecx)),(int) ((uint32_t*) (r->edx)), (int) ((uint32_t*) (r->esi)), (uint32_t) ((uint32_t*) (r->edi)));
            r->edx = (uint32_t)1;
            break;
        case SC_CODE_version:
            r->edx = (uint32_t)(VERSION_MAJOR * 100 + VERSION_MINOR);
            break;
        default: 
            qemu_printf("Invalid syscall #%x\n", r->eax);
            tty_printf("Invalid syscall #%x\n", r->eax);
    }
}