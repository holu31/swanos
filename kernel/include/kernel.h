#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

#include <multyboot.h>

#include <mem/mem.h>
#include <mem/pmm.h>
#include <mem/vmm.h>
#include <mem/kheap.h>

#include <drivers/vfs.h>
#include <drivers/ramdisk.h>
#include <drivers/keyboard.h>
#include <drivers/pci.h>
#include <drivers/ata.h>

#include <io/tty.h>
#include <io/vgafnt.h>
#include <io/ports.h>

#include <interrupts/gdt.h>
#include <interrupts/idt.h>
#include <interrupts/tss.h>
#include <interrupts/syscalls.h>

#include <libk/string.h>
#include <libk/stdlib.h>

#include <sys/elf.h>


#define VERSION_MAJOR   0   // kernel version
#define VERSION_MINOR   1   // pre-release
#define VERSION_PATCH   0   // patch

#define COLOR_TEXT      VESA_LIGHT_GREY
#define COLOR_SYS_TEXT  VESA_LIGHT_CYAN
#define COLOR_SYS_PATH  VESA_LIGHT_GREEN
#define COLOR_ERROR     VESA_LIGHT_RED
#define COLOR_ALERT     VESA_LIGHT_YELLOW

#define isDistr         0   // 0 - is not a distribution, 1 - is a distribution
#define DistrName       ""  // distribution name
#define dVERSION_MAJOR  0   // distribution version
#define dVERSION_MINOR  0   // distribution pre-release
#define dVERSION_PATCH  0   // distribution patch
extern int os_mode;


#define IRQ_START asm volatile("add $0x1c, %esp"); \
		asm volatile("pusha");

#define IRQ_END asm volatile("popa"); \
	asm volatile("iret");