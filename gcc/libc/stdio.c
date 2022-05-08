#include <stdio.h>

int puts(char *str){
    asm volatile("int $0x80" ::"a"(0), "b"(str));
    return 0;
}

int getscancode(){
    int result = -1;

    asm volatile("int $0x80" ::"a"(1));
    asm volatile("mov %%eax, %0" : "=a"(result));

    return result;
}

char getchar(){
    int result = -1;

    asm volatile("int $0x80" ::"a"(2));
    asm volatile("mov %%eax, %0" : "=a"(result));

    return result;
} 

char *gets() {
    void* res = 0;
    char *result = "";

    asm volatile("int $0x80" ::"a"(3));
    asm volatile("mov %%eax, %0" : "=a"(result));

    return result;
} 

int getversion(){
    int result = -1;
    

    asm volatile("int $0x80" ::"a"(4));
    asm volatile("mov %%eax, %0" : "=a"(result));

    return result;
}