#include <string.h>

size_t strlen(const char* str){
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

char *strchr(const char *s, const char c){
    while (*s && *s != c)
        ++s;
    return (*s) ? (char*) s : NULL;
}
 
char *strtok(char * str, const char * delim){        // separating a string by a character
    static char *next;
    
    if(str){
        next = str;
        while (*next && strchr(delim, *next))
            *next++ = '\0';
    }
    
    if (!*next)
        return NULL;
    
    str = next;
    
    while (*next && ! strchr(delim, *next))
        ++next;
    while (*next && strchr(delim, *next))
        *next++ = '\0';
    
    return str;
}

int strcmp(char *st1,char *st2){                       // checking the similarity of words
    int n = 0;
    while(*(st1+n) != '\0' ||*(st2+n) != '\0' )
    {
        if (*(st1+n) > *(st2+n))
        return 1;
        if(*(st1+n) < *(st2+n))
        return -1;
        n++;
    }
    return 0;
}

void *memset(void *destination, int c, size_t n){
    if(destination){
        char *tmp_mem = (char*)destination;
        while(n--) *tmp_mem++ = c;
    }

    return destination;
}

void *memcpy(void *destination, const void *source, size_t n){
    char *tmp_dest = (char*)destination;
    const char *tmp_src = (const char*)source;

    while(n){
        *tmp_dest++ = *tmp_src++;
        n--;
    }

    return destination;
}