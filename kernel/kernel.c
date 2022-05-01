/*
    SwanOS kernel
    Active participants in the development of the operating system:
     - Daniil Lebedev, github - https://github.com/holu31
     - Viktor Wiśniewski, github - https://github.com/vi-house-edvin
     - Mikhail Frolov, github - https://github.com/Doczom
*/

#include <kernel.h>


int os_mode = 1; // 0 - little RAM, 1 - default mode, 2 - boost mode, 3 - server

void kernel(int magic_number, struct multiboot_info *mboot_info) {
    tty_init(mboot_info);   // graphics settings
    tty_printf("\t\tSwan-OS\t%d.%d.%d (%s) x86_32\n", 
        VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH,    // kernel version
        __TIMESTAMP__                                   // kernel compilation end time
        );
    
    if (isDistr) {
        tty_printf("\tSwan-OS distribution \"%s\" %d.%d.%d\n",
            DistrName, dVERSION_MAJOR, dVERSION_MINOR, dVERSION_PATCH
            );
    }

    gdt_init(); // \    installing and configuring interrupts
    idt_init(); // /
    pmm_init(mboot_info);

    uint32_t initrd_beg = *(uint32_t*) (mboot_info->mods_addr);     // ramdisk start address
    uint32_t initrd_end = *(uint32_t*) (mboot_info->mods_addr + 4); // ramdisk end address
    qemu_printf("initrd_beg = %x initrd_end = %x\n", 
        initrd_beg, initrd_end              // outputting information about ramdisk addresses to the debugger
        );

    vmm_init();                             // initializing the virtual memory manager
    kheap_init();                           // Kernel heap initialization
    init_vbe(mboot_info);                   // 1024x768 graphics activation
    vfs_init();                             // initializing the virtual file system
    initrd_init(initrd_beg, initrd_end);    // initialization ramdisk
    syscall_init();                         // initialization of the system api for programs
    keyboard_install();                     // keyboard driver installation
    pci_init();                             // PCI driver installation

    ksh_main();                             // terminal activation
}
