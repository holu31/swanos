#ifndef _MEM_H_
#define _MEM_H_

#include <stdint.h>

// Constants to the boundaries of memory
#define KERNEL_START_PADDR ((uint32_t) &kernel_phys_start)
#define KERNEL_END_PADDR   ((uint32_t) &kernel_phys_end)
#define KERNEL_SIZE        (KERNEL_END_PADDR - KERNEL_START_PADDR)

#define KERNEL_START_VADDR ((uint32_t) &kernel_virt_start)
#define KERNEL_END_VADDR   ((uint32_t) &kernel_virt_end)

#define KERNEL_PHYS_MAP_START kernel_phys_map_start
#define KERNEL_PHYS_MAP_END   kernel_phys_map_end
#define KERNEL_PHYS_MAP_SIZE  kernel_phys_map_end - kernel_phys_map_start;

// Constants for the Physical Memory Manager
#define PHYS_BLOCKS_PER_BYTE 8
#define PHYS_BLOCK_SIZE      4096

// Constants for the Virtual Memory Manager
#define TEMP_PAGE_ADDR  0xC03FF000 // 0x1000 is reserved i.e 4096 bytes up to 0xC0400000 for temporary page
#define PAGES_PER_TABLE 1024
#define PAGES_PER_DIR   1024
#define PAGE_SIZE       4096
#define PAGE_ENTRIES    1024

// Constants for Kernel heap manager
#define KHEAP_START_VADDR 0xC0500000 // Change if kernel size > 4MB

// Constants for User heap manager
#define UHEAP_START_VADDR 0x20000000 // Virtual address where user heap in each process page dir begins

// Functions to
#define ALIGN_BLOCK(addr) (addr) - ((addr) % PHYS_BLOCK_SIZE);

// Addresses of the Kernel in the Physical and Virtual memory
extern uint32_t kernel_phys_start;
extern uint32_t kernel_phys_end;
extern uint32_t kernel_virt_start;
extern uint32_t kernel_virt_end;

// Addresses of the Physical Memory Map in the Physical memory
extern uint32_t kernel_phys_map_start;
extern uint32_t kernel_phys_map_end;

typedef uint32_t virtual_addr;
typedef uint32_t physical_addr;

#define MBOOT_FLAG_MEM     0x001
#define MBOOT_FLAG_DEVICE  0x002
#define MBOOT_FLAG_CMDLINE 0x004
#define MBOOT_FLAG_MODS    0x008
#define MBOOT_FLAG_AOUT    0x010
#define MBOOT_FLAG_ELF     0x020
#define MBOOT_FLAG_MMAP    0x040
#define MBOOT_FLAG_CONFIG  0x080
#define MBOOT_FLAG_LOADER  0x100
#define MBOOT_FLAG_APM     0x200
#define MBOOT_FLAG_VBE     0x400

struct multiboot_info {
    uint32_t flags; /* Header flags */
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    uint32_t num;
    uint32_t size;
    uint32_t addr;
    uint32_t shndx;
    uint32_t mmap_length;
    uint32_t mmap_addr;
    uint32_t drives_length;
    uint32_t drives_addr;
    uint32_t config_table;
    uint32_t boot_loader_name;
    uint32_t apm_table;
    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint32_t vbe_mode;
    uint32_t vbe_interface_seg;
    uint32_t vbe_interface_off;
    uint32_t vbe_interface_len;

    uint64_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t framebuffer_bpp;
    uint8_t framebuffer_type;

    union {
        struct {
            uint32_t framebuffer_palette_addr;
            uint16_t framebuffer_palette_num_colors;
        };

        struct {
            uint8_t framebuffer_red_field_position;
            uint8_t framebuffer_red_mask_size;
            uint8_t framebuffer_green_field_position;
            uint8_t framebuffer_green_mask_size;
            uint8_t framebuffer_blue_field_position;
            uint8_t framebuffer_blue_mask_size;
        };
    };
};

typedef struct multiboot_info multiboot_info;

struct multiboot_memory_map_entry {
    uint32_t size;
    uint64_t addr;
    uint64_t len;
    uint32_t type;
};

typedef struct multiboot_memory_map_entry multiboot_memory_map_entry;

#endif