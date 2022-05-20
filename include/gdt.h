#pragma once
#include <stdint.h>

struct gdt_entry_struct {
    uint16_t limit_low;       // lower 16 bits of the limit
    uint16_t base_low;        // lower 16 bits of the base
    uint8_t base_middle;      // next 8 bits of the base
    uint8_t access;           // Access flags determine what ring this segment can be used in
    uint8_t granularity;
    uint8_t base_high;        // last 8 bits of the base
} __attribute__((packed));

struct gdt_ptr_struct {
    uint16_t limit;           // upper 16 bits of all selector limits
    uint32_t base;            // address of the first gdt_entry_t struct
} __attribute__((packed));

void gdt_init();