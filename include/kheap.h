#ifndef _KHEAP_H_
#define _KHEAP_H_
#include <stdint.h>
#include <stdbool.h>

// constants to the boundaries of memory
#define KERNEL_START_PADDR ((uint32_t) &kernel_phys_start)
#define KERNEL_END_PADDR   ((uint32_t) &kernel_phys_end)
#define KERNEL_SIZE        (KERNEL_END_PADDR - KERNEL_START_PADDR)

#define KERNEL_START_VADDR ((uint32_t) &kernel_virt_start)
#define KERNEL_END_VADDR   ((uint32_t) &kernel_virt_end)

#define KERNEL_PHYS_MAP_START kernel_phys_map_start
#define KERNEL_PHYS_MAP_END   kernel_phys_map_end
#define KERNEL_PHYS_MAP_SIZE  kernel_phys_map_end - kernel_phys_map_start;

// constants for the PMM
#define PHYS_BLOCKS_PER_BYTE 8
#define PHYS_BLOCK_SIZE      4096

// constants for the VMM
#define TEMP_PAGE_ADDR  0xC03FF000 // 0x1000 is reserved i.e 4096 bytes up to 0xC0400000 for temporary page
#define PAGES_PER_TABLE 1024
#define PAGES_PER_DIR   1024
#define PAGE_SIZE       4096
#define PAGE_ENTRIES    1024

#define KHEAP_START_VADDR 0xC0500000 // Change if kernel size > 4MB
#define UHEAP_START_VADDR 0x20000000 // Virtual address where user heap in each process page dir begins

#define ALIGN_BLOCK(addr) (addr) - ((addr) % PHYS_BLOCK_SIZE);

extern uint32_t kernel_phys_start;
extern uint32_t kernel_phys_end;
extern uint32_t kernel_virt_start;
extern uint32_t kernel_virt_end;

extern uint32_t kernel_phys_map_start;
extern uint32_t kernel_phys_map_end;

typedef uint32_t virtual_addr;
typedef uint32_t physical_addr;

typedef struct __attribute__((packed)) kheap_item { //9 bytes yes?
    struct kheap_item *next;
    uint32_t size;
    unsigned char used;
} kheap_item;

void kheap_init();

void *kheap_morecore(uint32_t size);
int32_t kheap_free(void *address);
void *kheap_malloc(uint32_t size);

#endif