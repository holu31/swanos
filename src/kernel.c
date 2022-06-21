#include <console.h>
#include <gdt.h>
#include <idt.h>
#include <irq.h>
#include <isr.h>
#include <kheap.h>
#include <vmm.h>
#include <pmm.h>
#include <kernel.h>

void kernel_main(struct multiboot_info *mboot_info){
	cinit();
	gdt_init();
	idt_init();
	irq_init();
	isrs_init();
	pmm_init(mboot_info);
	kheap_init();
	while(1);
}