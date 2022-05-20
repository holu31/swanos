#include <console.h>
#include <gdt.h>
#include <idt.h>

void kernel_main(){
	cinit();
	gdt_init();
	idt_init();
	while(1);
}