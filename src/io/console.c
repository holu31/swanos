#include <console.h>
#include <ports.h>


static uint16_t* buffer;


void update_cursor(){
    uint16_t pos = row * 80 + column;
 
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t) (pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}


void cset_color(uint8_t colfg, uint8_t colbg){
    color = colfg | colbg << 4;
}


void cinit(void) {
    row = 0;
    column = 0;
    color = COLOR_LIGHT_GREY | COLOR_BLACK << 4;
    buffer = (uint16_t*) 0xC03FF000;
    for (size_t y = 0; y < 25; y++){
        for (size_t x = 0; x < 80; x++){
            const size_t index = y * 80 + x;
            buffer[index] = (uint16_t) ' ' | (uint16_t) color << 8;
        }
    }
}


void terminal_delete_last_line() {
	int *ptr;
 
	for(int x = 0; x < 80 * 2; x++) {
		ptr = 0xC03FF000 + (80 * 2) * (25 - 1) + x;
		*ptr = 0;
	}
}


void terminal_scroll(uint8_t line) {
    int *loop;
	char c;
 
	for(loop = line * (80 * 2) + 0xC03FF000; loop < 80 * 2; loop++) {
		c = *loop;
		*(loop - (80 * 2)) = c;
	}
}


void cputch(char c){
    const size_t index = row * 80 + column;
    if(c != '\n') buffer[index] = (uint16_t) c | (uint16_t) color << 8;
    if (++column == 80 || c == '\n') {
        column = 0;
        if (++row >= 25){
			for(uint8_t line = 1; line <= 25; line++) {
				terminal_scroll(line);
			}
			terminal_scroll(5);
			terminal_delete_last_line();
			terminal_delete_last_line();
			row = 23;
        }
    }
    update_cursor();
}


void cputs(char *c){
    for(int i=0; c[i]!='\0';i++)
        cputch(c[i]);
}


void cputint(int i){
    if(i < 0){
        i = -i;
        cputch('-');
    }
    char str[255];
    uint32_t n, d = 1000000000;
    uint32_t dec_index = 0;

    while((i / d == 0) && (d >= 10)){
        d /= 10;
    }
    n = i;

    while(d >= 10){
        str[dec_index++] = ((char)((int) '0' + n/d));
        n = n % d;
        d /= 10;
    }

    str[dec_index++] = ((char) ((int) '0' + n));
    str[dec_index] = 0;
    cputs(str);
}


void cbackspace(){
    column -= 1;
    const size_t index = row * 80 + column;
    buffer[index] = (uint16_t) ' ' | (uint16_t) color << 8;
    cputch(' ');
    column -= 1;
    update_cursor();
}


void log(char *str, bool ok){
    cputs("[");
    cset_color(COLOR_LIGHT_GREEN, COLOR_BLACK);
    if(ok) cputs("OK");
    else {
        cset_color(COLOR_LIGHT_RED, COLOR_BLACK);
        cputs("ERR");
    }
    cset_color(COLOR_LIGHT_GREY, COLOR_BLACK);
    cputs("] ");
    cputs(str);
}