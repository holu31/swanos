#include <gdt.h>
#include <console.h>
#include <kernel.h>

struct gdt_entry_struct gdt_entries[6];
struct gdt_ptr_struct gdt_ptr;

extern void gdt_flush(struct gdt_ptr_struct *gdt_ptr_addr);

void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran){  // set value of one GDT entry
    gdt_entries[num].base_low     =  (base & 0xFFFF);
    gdt_entries[num].base_middle  =  (base >> 16) & 0xFF;
    gdt_entries[num].base_high    =  (base >> 24) & 0xFF;

    gdt_entries[num].limit_low    =  (limit & 0xFFFF);
    gdt_entries[num].granularity  =  ((limit >> 16) & 0x0F);

    gdt_entries[num].granularity  |= (gran & 0xF0);
    gdt_entries[num].access       = access;
}

void gdt_init() {
    gdt_ptr.limit = (sizeof(struct gdt_entry_struct) * 6) - 1;
    gdt_ptr.base = (uint32_t) &gdt_entries;

    gdt_set_gate(0, 0, 0, 0, 0);                    // null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);     // code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);     // data segment
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);     // user mode code segment
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);     // user mode data segment
    gdt_flush(&gdt_ptr);
    if(DEBUG_MODE) log("gdt init\n", true);
}