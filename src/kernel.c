#include <console.h>
#include <gdt.h>
#include <idt.h>
#include <irq.h>
#include <isr.h>
#include <kernel.h>
#include <shell.h>

void kernel_main(){
	cinit();
	gdt_init();
	idt_init();
	irq_init();
	isrs_init();
	shell_init();
}
