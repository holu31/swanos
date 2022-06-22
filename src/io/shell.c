#include <shell.h>
#include <keyboard.h>
#include <console.h>
#include <string.h>
#define SIZE_BUFFER 80

char *get_command(){
    char buffer_shell[SIZE_BUFFER];
    int key = kb_getkey();
    cset_color(COLOR_CYAN, COLOR_BLACK);
    cputs("\n~$ ");
    cset_color(COLOR_LIGHT_GREY, COLOR_BLACK);
    while(kb_sctochar(key) != '\n'){
        if(kb_sctochar(key) != 0){
            buffer_shell[sizeof(buffer_shell)] = kb_sctochar(key);
            cputch(buffer_shell[sizeof(buffer_shell)]);
        }
        key = kb_getkey();
    }
    return buffer_shell;
}

void shell_init(){
    while(1){
        char *command = get_command(); // requires a memory manager in order to work properly
    }
}