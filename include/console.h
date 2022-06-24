#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
 
enum colors {
    COLOR_BLACK = 0,
    COLOR_BLUE = 1,
    COLOR_GREEN = 2,
    COLOR_CYAN = 3,
    COLOR_RED = 4,
    COLOR_MAGENTA = 5,
    COLOR_BROWN = 6,
    COLOR_LIGHT_GREY = 7,
    COLOR_DARK_GREY = 8,
    COLOR_LIGHT_BLUE = 9,
    COLOR_LIGHT_GREEN = 10,
    COLOR_LIGHT_CYAN = 11,
    COLOR_LIGHT_RED = 12,
    COLOR_LIGHT_MAGENTA = 13,
    COLOR_LIGHT_YELLOW = 14,
    COLOR_WHITE = 15,
};
 
size_t row;
size_t column;
uint8_t color;
uint16_t* buffer;

void cinit(void);
void cputch(char c);
void cset_color(uint8_t colfg, uint8_t colbg);
void cputs(char *c);
void cputint(int i);
void cbackspace();
void log(char *str, bool ok);