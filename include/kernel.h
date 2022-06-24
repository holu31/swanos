#pragma once
#ifndef KERNEL_H
#define KERNEL_H
#include <stdint.h>
#define DEBUG_MODE 1
#define KERNEL_NAME "SWAN-OS"
#define KERNEL_VERSION_STRING "0.1.2"
#define NULL ((void *)0)

struct regs {
	unsigned int gs, fs, es, ds;
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	unsigned int int_no, err_code;
	unsigned int eip, cs, eflags, useresp, ss;
};

#endif
