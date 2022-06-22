#pragma once
#ifndef ISR_H
#define ISR_H
#define KB_DATA_PORT  0x60
#include <stdint.h>

uint8_t kb_getkey();
char kb_getchar();
char kb_sctochar(int scancode);

#endif