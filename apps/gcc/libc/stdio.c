#include <stdio.h>

int puts(char *str){
    int result = -1;

    asm volatile("int $0x80" : "=a"(result) : "a"(0), "b"(str));
    return result;
}

int getscancode(){
    int result = -1;

    asm volatile("int $0x80" : "=a"(result) : "a"(1));
    return result;
}

char getchar(){
    int result = -1;

    asm volatile("int $0x80" : "=a"(result) : "a"(2));
    return result;
} 

char *gets() {
    char *result = "";

    asm volatile("int $0x80" : "=a"(result) : "a"(3));
    return result;
} 

int getversion(){
    int result = -1;

    asm volatile("int $0x80" : "=a"(result) :"a"(4));
    return result;
}