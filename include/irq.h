#pragma once
#include <kernel.h>
#ifndef IRQ_H
#define IRQ_H

typedef void (*irq_handler_t) (struct regs *);
void irq_init();
void irq_install_handler(int irq, irq_handler_t handler);
void irq_uninstall_handler(int irq);

#endif