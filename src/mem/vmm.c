/*
    Taken from https://github.com/rgimad/EOS/tree/d3e2062fc909d8b15d8637950050281f051270d2
    under license MIT license
*/

#include <vmm.h>
#include <console.h>
#include <string.h>
#include <kernel.h>
#include <ports.h>
#include <pmm.h>

page_directory *kernel_page_dir; // pointer (physical) to kernel page dircetory structure

bool vmm_alloc_page(virtual_addr vaddr){
    physical_addr paddr = pmm_alloc_block();
    if(!paddr){
        return false;
    }
    vmm_map_page(paddr, vaddr);
    return true;
}

bool vmm_alloc_page_with_userbit(virtual_addr vaddr){
    physical_addr paddr = pmm_alloc_block();
    if(!paddr){
        return false;
    }

    vmm_map_page(paddr, vaddr);
    page_table_entry *pte = GET_PTE(vaddr);
    page_table_entry_add_attrib(pte, I86_PTE_USER);
    return true;
}

void vmm_free_page(virtual_addr vaddr) {
    page_table_entry *pte = GET_PTE(vaddr);
    if (!page_table_entry_is_present(*pte)) {
        return;
    }

    physical_addr block = page_table_entry_frame(*pte);
    if (block) {
        pmm_free_block(block);
    }
    page_table_entry_del_attrib(pte, I86_PTE_PRESENT);
}

void vmm_create_kernel_page_dir() {
    kernel_page_dir = (page_directory*) pmm_alloc_block();
    if (kernel_page_dir == 0xFFFFFFFF) {
        return;
    }

    page_directory *pd = kernel_page_dir;
    memset(pd, 0, sizeof(page_directory));

    int i;
    for(i = 0; i < PAGE_ENTRIES; i++){
        page_dir_entry *pde = (page_dir_entry*) &pd->entries[i];
        page_dir_entry_add_attrib(pde, I86_PTE_WRITABLE);
        page_dir_entry_del_attrib(pde, I86_PTE_PRESENT);

        if(i == PAGE_ENTRIES - 1){
            page_dir_entry_add_attrib(pde, I86_PTE_PRESENT);
            page_dir_entry_set_frame(pde, (physical_addr) kernel_page_dir);
        }
    }
}

void vmm_map_page(physical_addr paddr, virtual_addr vaddr){
    page_dir_entry *pde = GET_PDE(vaddr);
    if(!page_dir_entry_is_present(*pde)){
        physical_addr pt_p = pmm_alloc_block();
        if(pt_p == 0xFFFFFFFF){
            return;
        }

        page_table *pt_v = (page_table*) vmm_temp_map_page(pt_p);
        memset(pt_v, 0, sizeof(page_table));
        page_dir_entry_add_attrib(pde, I86_PDE_PRESENT);
        page_dir_entry_add_attrib(pde, I86_PDE_WRITABLE);
        page_dir_entry_set_frame(pde, pt_p);
    }

    page_table_entry *pte = GET_PTE(vaddr);
    page_table_entry_set_frame(pte, paddr);
    page_table_entry_add_attrib(pte, I86_PTE_PRESENT);
    page_table_entry_add_attrib(pte, I86_PTE_WRITABLE);
    flush_tlb_entry(vaddr);
}

virtual_addr vmm_temp_map_page(physical_addr paddr){
    page_table_entry *pte = GET_PTE(TEMP_PAGE_ADDR);
    page_table_entry_set_frame(pte, PAGE_ALIGN_DOWN(paddr)); 
    page_table_entry_add_attrib(pte, I86_PTE_PRESENT);
    page_table_entry_add_attrib(pte, I86_PTE_WRITABLE);
    asm volatile("invlpg %0" :: "m" (*(uint32_t *) TEMP_PAGE_ADDR) : "memory" );

    return TEMP_PAGE_ADDR;
}

void vmm_switch_page_directory(page_directory *page_dir_phys_addr){
    asm volatile("mov %0, %%cr3" :: "r"((uint32_t) page_dir_phys_addr));
}

void vmm_init(){

    vmm_create_kernel_page_dir();
    page_table *table1 = (page_table*) pmm_alloc_block();
    page_table *table2 = (page_table*) pmm_alloc_block();

    memset((void*) table1, 0, sizeof(page_table));
    memset((void*) table2, 0, sizeof(page_table));

    physical_addr frame;
    virtual_addr virt;
    for(frame = 0x0, virt = 0xC0000000;
         frame < 0x100000/*0x100000*/;
         frame += PAGE_SIZE, virt += PAGE_SIZE){
        page_table_entry page = 0;
        page_table_entry_add_attrib(&page, I86_PTE_PRESENT);
        page_table_entry_set_frame(&page, frame);
        table1->entries[PAGE_TABLE_INDEX(virt)] = page;
    }

    // maps kernel pages and phys mem pages
    for(frame = KERNEL_START_PADDR, virt = KERNEL_START_VADDR;
         frame < KERNEL_PHYS_MAP_END;
         frame += PAGE_SIZE, virt += PAGE_SIZE){
        page_table_entry page = 0;
        page_table_entry_add_attrib(&page, I86_PTE_PRESENT);
        page_table_entry_set_frame(&page, frame);
        table2->entries[PAGE_TABLE_INDEX(virt)] = page;
    }

    page_dir_entry *pde1 = (page_dir_entry*) &kernel_page_dir->entries[PAGE_DIRECTORY_INDEX(0x00000000)]; //pdirectory_lookup_entry(cur_directory, 0x00000000);
    page_dir_entry_add_attrib(pde1, I86_PDE_PRESENT);
    page_dir_entry_add_attrib(pde1, I86_PDE_WRITABLE);
    page_dir_entry_set_frame(pde1, (physical_addr) table1);

    page_dir_entry **pde2 = (page_dir_entry**) &kernel_page_dir->entries[PAGE_DIRECTORY_INDEX(0xC0100000)]; //pdirectory_lookup_entry(cur_directory, 0xC0100000);
    page_dir_entry_add_attrib(pde2, I86_PDE_PRESENT);
    page_dir_entry_add_attrib(pde2, I86_PDE_WRITABLE);
    page_dir_entry_set_frame(pde2, (physical_addr) table2);

    update_phys_memory_bitmap_addres(KERNEL_END_VADDR);

    enable_paging((physical_addr) kernel_page_dir);

    if(DEBUG_MODE) log("VMM init", true);
}

// Add attribute to pte
void page_table_entry_add_attrib(page_table_entry *entry, uint32_t attrib){
    *entry |= attrib;
}

// delete attribute to pte
void page_table_entry_del_attrib(page_table_entry *entry, uint32_t attrib){
    *entry &= ~attrib;
}

// dap pte to physical frame
void page_table_entry_set_frame(page_table_entry *entry, physical_addr addr){
    *entry = (*entry & ~I86_PTE_FRAME) | addr;
}

bool page_table_entry_is_present(page_table_entry entry){
    return entry & I86_PTE_PRESENT;
}

bool page_table_entry_is_writable(page_table_entry entry){
    return entry & I86_PTE_WRITABLE;
}

// return the address of physical frame which pte refers to
physical_addr page_table_entry_frame(page_table_entry entry){
    return entry & I86_PTE_FRAME;
}

// functions for Page Directory Entries

// add attribute to pde
void page_dir_entry_add_attrib(page_dir_entry *entry, uint32_t attrib){
    *entry |= attrib;
}

// delete attribute to pde
void page_dir_entry_del_attrib(page_dir_entry *entry, uint32_t attrib){
    *entry &= ~attrib; //old: was without ~ !!
}

// map pde to physical frame (where the appropriate page table stores)
void page_dir_entry_set_frame(page_dir_entry *entry, physical_addr addr){
    *entry = (*entry & ~I86_PDE_FRAME) | addr;
}

bool page_dir_entry_is_present(page_dir_entry entry){
    return entry & I86_PDE_PRESENT;
}

bool page_dir_entry_is_user(page_dir_entry entry){
    return entry & I86_PDE_USER;
}

bool page_dir_entry_is_4mb(page_dir_entry entry){
    return entry & I86_PDE_4MB;
}

bool page_dir_entry_is_writable(page_dir_entry entry){
    return entry & I86_PDE_WRITABLE;
}

// return the address of physical frame which pde refers to
physical_addr page_dir_entry_frame(page_dir_entry entry){
    return entry & I86_PDE_FRAME;
}

void flush_tlb_entry(virtual_addr addr){
    asm volatile("invlpg (%0)" : : "b"(addr) : "memory");
}