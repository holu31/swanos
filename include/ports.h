#ifndef PORTS_H
#define PORTS_H

#include <stdint.h>

#define PORT_COM1 0x3f8

uint8_t  inb(uint16_t port);//read a byte from port
uint16_t inw(uint16_t port);//read a word from port
uint32_t inl(uint16_t port);//read a dword from port
void outb(uint16_t port, uint8_t  val);//write a byte into port
void outw(uint16_t port, uint16_t val);//write a word into port
void outl(uint16_t port, uint32_t val);//write a dword into port
uint64_t rdtsc();//get the the number of cycles since cpu reset
void insl(unsigned short port, unsigned int buffer, unsigned long count);//read <count> unsignedint's(=dword?) from <port> to <buffer>

void io_wait(void);
void sleep(int32_t i);

#endif