#pragma once
#include <stdint.h>

struct idt_entry_struct {
    uint16_t base_lo;      // lower 16 bits of the address to jump to when this interrupt fires
    uint16_t sel;          // kernel segment selector
    uint8_t  always0;      // must always be zero
    uint8_t  flags;        // more flags
    uint16_t base_hi;      // upper 16 bits of the addres to jump to
} __attribute__((packed));

struct idt_ptr_struct {
    uint16_t limit; 
    uint32_t base;            // address of the first element in our idt_entry array
} __attribute__((packed));


void idt_init();
void idt_set_gate(uint32_t num, uint32_t base, uint16_t sel, uint8_t flags);