#include <console.h>
#include <gdt.h>
#include <idt.h>
#include <irq.h>
#include <isr.h>

void kernel_main(){
	cinit();
	gdt_init();
	idt_init();
	irq_init();
	isrs_init();
	while(1);
}