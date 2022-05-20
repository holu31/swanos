#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

size_t strlen(const char* str);
char *strchr(const char *s, const char c);
char *strtok(char * str, const char * delim);
int strcmp(char *st1,char *st2);
void *memset(void *destination, int c, size_t n);
void *memcpy(void *destination, const void *source, size_t n);