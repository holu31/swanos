#include <mem/mem.h>

uint64_t phys_installed_memory_size, phys_available_memory_size;
void pmm_init(struct multiboot_info *mboot_info);