#include <stdio.h>

int putchar(char ch){
    asm volatile("mov $6, %eax");
    asm volatile("mov %0, %%ebx" :: "b"((int) ch) : "%eax");
    asm volatile("int $0x80");
    return 0;
}

int puts(char *str){
    for(int i=0; str[i] != '\0'; i++){
        putchar(str[i]);
    }
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