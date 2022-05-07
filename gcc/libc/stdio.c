#include <stdio.h>

int puts(char *ch){
    asm volatile("mov $0, %eax");
    asm volatile("mov %0, %%ebx" :: "b"(ch) : "%eax");
    asm volatile("int $0x80");
    return 0;
}

int getscancode(){
    int result = -1;

    asm volatile("mov %%eax, %0" :: "a"(1));
    asm volatile("int $0x80");
    asm volatile("mov %%edx, %0" : "=a"(result));

    return result;
}

char getchar(){
    int result = -1;

    asm volatile("mov %%eax, %0" :: "a"(2));
    asm volatile("int $0x80");
    asm volatile("mov %%edx, %0" : "=a"(result));

    return result;
} 

char *gets() {
    void* res = 0;
    char *result = "";

    asm volatile("mov %%eax, %0" :: "a"(3));
    asm volatile("int $0x80");
    asm volatile("mov %%edx, %0" : "=a"(result));

    return result;
} 

int getversion(){
    int result = -1;
    

    asm volatile("mov %%eax, %0" :: "a"(40));
    asm volatile("int $0x80");
    asm volatile("mov %%edx, %0" : "=a"(result));

    return result;
}