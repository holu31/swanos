#pragma once
#ifndef KHEAP_H
#define KHEAP_H
#include <stdbool.h>
#include <stdint.h>

#define PHYS_BLOCKS_PER_BYTE 8
#define PHYS_BLOCK_SIZE      4096

#define TEMP_PAGE_ADDR  0xC03FF000 
#define PAGES_PER_TABLE 1024
#define PAGES_PER_DIR   1024
#define PAGE_SIZE       4096
#define PAGE_ENTRIES    1024

#define KHEAP_START_VADDR 0xC0500000 

#define UHEAP_START_VADDR 0x20000000 

#define ALIGN_BLOCK(addr) (addr) - ((addr) % PHYS_BLOCK_SIZE);

typedef struct __attribute__((packed)) kheap_item { 
    struct kheap_item *next;
    uint32_t size;
    unsigned char used;
} kheap_item;


void kheap_init();
int32_t kheap_free(void *address);
void *kheap_malloc(uint32_t size);
#endif