#include <interrupts/idt.h>


#define SC_CODE_puts            10
#define SC_CODE_getscancode     11
#define SC_CODE_getchar         12
#define SC_CODE_gets            13
#define SC_CODE_malloc          14
#define SC_CODE_free            15
#define SC_CODE_putpixel        32
#define SC_CODE_drawline        33
#define SC_CODE_version         40

void syscall_init();
void syscall_handler(struct regs *r);