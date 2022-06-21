#include <idt.h>
#include <kernel.h>
#include <irq.h>
#include <ports.h>
#include <console.h>

extern void _irq0();
extern void _irq1();
extern void _irq2();
extern void _irq3();
extern void _irq4();
extern void _irq5();
extern void _irq6();
extern void _irq7();
extern void _irq8();
extern void _irq9();
extern void _irq10();
extern void _irq11();
extern void _irq12();
extern void _irq13();
extern void _irq14();
extern void _irq15();

static irq_handler_t irq_routines[16] = { NULL };

void irq_install_handler(int irq, irq_handler_t handler){
	irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq){
	irq_routines[irq] = 0;
}

void irq_remap(){
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x0);
	outb(0xA1, 0x0);
}

void irq_gates(){
	idt_set_gate(32, (uint32_t)_irq0, 0x08, 0x8E);
	idt_set_gate(33, (uint32_t)_irq1, 0x08, 0x8E);
	idt_set_gate(34, (uint32_t)_irq2, 0x08, 0x8E);
	idt_set_gate(35, (uint32_t)_irq3, 0x08, 0x8E);
	idt_set_gate(36, (uint32_t)_irq4, 0x08, 0x8E);
	idt_set_gate(37, (uint32_t)_irq5, 0x08, 0x8E);
	idt_set_gate(38, (uint32_t)_irq6, 0x08, 0x8E);
	idt_set_gate(39, (uint32_t)_irq7, 0x08, 0x8E);
	idt_set_gate(40, (uint32_t)_irq8, 0x08, 0x8E);
	idt_set_gate(41, (uint32_t)_irq9, 0x08, 0x8E);
	idt_set_gate(42, (uint32_t)_irq10, 0x08, 0x8E);
	idt_set_gate(43, (uint32_t)_irq11, 0x08, 0x8E);
	idt_set_gate(44, (uint32_t)_irq12, 0x08, 0x8E);
	idt_set_gate(45, (uint32_t)_irq13, 0x08, 0x8E);
	idt_set_gate(46, (uint32_t)_irq14, 0x08, 0x8E);
	idt_set_gate(47, (uint32_t)_irq15, 0x08, 0x8E);
}

void irq_init(){ // setup hardware devices
	irq_remap();
	irq_gates();
	if(DEBUG_MODE) log("irq init\n", true);
	__asm__ __volatile__("sti");
}

void irq_ack(int irq_no){
	if(irq_no >= 12){
		outb(0xA0, 0x20);
	}
	outb(0x20, 0x20);
}

void irq_handler(struct regs *r){
	__asm__ __volatile__("cli");
	void (*handler)(struct regs *r);
	if(r->int_no > 47 || r->int_no < 32){
		handler = NULL;
	} else {
		handler = irq_routines[r->int_no - 32];
	}
	if(handler){
		handler(r);
	} else {
		irq_ack(r->int_no - 32);
	}
	__asm__ __volatile__("sti");
}