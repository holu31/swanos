#pragma once
#ifndef KEYBOARD_H
#define KEYBOARD_H
#define KB_DATA_PORT  0x60
#include <stdint.h>

int kb_getkey();
char kb_getchar();
char kb_sctochar(int scancode);

#endif