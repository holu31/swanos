#pragma once
#include <irq.h>
#ifndef ISR_H
#define ISR_H

void isrs_install_handler(int isrs, irq_handler_t handler);
void isrs_uninstall_handler(int isrs);
void isrs_init();

#endif