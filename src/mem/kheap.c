#include <kheap.h>
#include <console.h>
#include <mem.h>
#include <kernel.h>
#include <vmm.h>
#include <string.h>

virtual_addr kheap_begin; 
virtual_addr kheap_end; 

uint32_t kheap_memory_used; 
int32_t kheap_alloc_number;


void kheap_init() {
    kheap_begin = KHEAP_START_VADDR;
    kheap_end = (virtual_addr)NULL;

    kheap_alloc_number = 0;
    kheap_memory_used = 0;
    log("KHEAP init", true);
}


// increasing the kernel heap by some size, it will be rounded up to the page size
void *kheap_morecore(uint32_t size) {
    if(kheap_end == (virtual_addr)NULL){ 
        kheap_end = kheap_begin;
    }

    void *first_kheap_end = (void*)kheap_end; 
    
    // create page
    for (int32_t pages = (size / PAGE_SIZE) + 1; pages-- > 0; kheap_end += PAGE_SIZE) {
        vmm_alloc_page(kheap_end);
        memset((void*)kheap_end, 0x00, PAGE_SIZE);
    }

    return first_kheap_end;
}

int32_t kheap_free(void *address) {
    kheap_item *temp_item, *item;

    if(address == NULL){
        return -1;
    }

    item = (kheap_item*) ((uint32_t) address - (uint32_t) sizeof(kheap_item));

    for(temp_item = (kheap_item*)kheap_begin; temp_item != NULL; temp_item = temp_item->next){
        if(temp_item == item){
            temp_item->used = 0;
            kheap_memory_used -= temp_item->size;
            kheap_alloc_number--;

            for(temp_item = (kheap_item*)kheap_begin; temp_item != NULL; temp_item = temp_item->next){
                while(!temp_item->used && temp_item->next != NULL && !temp_item->next->used){
                    temp_item->size += sizeof(kheap_item) + temp_item->next->size;
                    temp_item->next = temp_item->next->next;
                }
            }
            return 1;
        }
    }
    return -1;
}

void *kheap_malloc(uint32_t size) {
    kheap_item *new_item = NULL, *temp_item;
    uint32_t total_size;

    if (size == 0) {
        return NULL;
    }

    total_size = ((size + 7) & ~7) + sizeof(kheap_item); // round by 8 bytes and add header size

    kheap_item *last_item;
    if (kheap_end != (virtual_addr)NULL) {
        for (new_item = (kheap_item*)kheap_begin; new_item != NULL; new_item = new_item->next) {
            if (new_item->next == NULL) {
                last_item = new_item;
            }
            if (!new_item->used && (total_size <= new_item->size)) {
                break;
            }
        }
    }

    if (new_item != NULL){
        temp_item = (kheap_item*) ((uint32_t) new_item + total_size);
        temp_item->size = new_item->size - total_size;
        temp_item->used = 0;
        temp_item->next = new_item->next;
    } else {
        new_item = kheap_morecore(total_size);
        if (new_item == NULL) {
            return NULL;
        }

        temp_item = (kheap_item*) ((uint32_t) new_item + total_size);
        temp_item->size = PAGE_SIZE - (total_size % PAGE_SIZE 
                                      ? total_size % PAGE_SIZE 
                                      : total_size) - sizeof(kheap_item);
        temp_item->used = 0;
        temp_item->next = NULL;

        last_item->next = new_item;
    }

    new_item->size = size;
    new_item->used = 1;
    new_item->next = temp_item;

    kheap_alloc_number++;
    kheap_memory_used += total_size;

    return (void*) ((uint32_t) new_item + (uint32_t) sizeof(kheap_item));
}