#include <console.h>
#include <idt.h>
#include <string.h>
#include <kernel.h>

extern void idt_load(struct idt_ptr_struct *idt_ptr_addr);

struct idt_entry_struct idt_entries[256];
struct idt_ptr_struct   idt_ptr;

void idt_set_gate(uint32_t num, uint32_t base, uint16_t sel, uint8_t flags){
	idt_entries[num].base_lo   = (base & 0xFFFF);
	idt_entries[num].base_hi   = (base >> 16) & 0xFFFF;
	idt_entries[num].sel       =  sel;
	idt_entries[num].always0   =  0;
	idt_entries[num].flags     = flags | 0x60;
}

/*
 * idt_install
 * Install the IDTs
 */
void idt_init() {
	idt_ptr.limit = (sizeof(struct idt_entry_struct) * 256) - 1;
	idt_ptr.base  = (uintptr_t)&idt_entries;
	memset(&idt_entries, 0, sizeof(struct idt_entry_struct) * 256);

	idt_load(&idt_ptr);
    if(DEBUG_MODE) log("idt init\n", true);
}
