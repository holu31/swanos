#include <shell.h>
#include <keyboard.h>
#include <console.h>
#include <string.h>
#define SIZE_BUFFER 80

char buffer_shell[SIZE_BUFFER];

char *get_command(){
    int key = kb_getkey();
    cset_color(COLOR_CYAN, COLOR_BLACK);
    cputs("\n~$ ");
    cset_color(COLOR_LIGHT_GREY, COLOR_BLACK);
    for(int i=0; i!=sizeof(buffer_shell); i++) buffer_shell[i] = '\0';
    while(kb_sctochar(key) != '\n'){
        if(kb_sctochar(key) != 0){
            buffer_shell[strlen(buffer_shell)] = kb_sctochar(key);
            cputch(kb_sctochar(key));
        }
        key = kb_getkey();
    }
    return buffer_shell;
}

void shell_init(){
    while(1){
        char *command = get_command(); // requires a memory manager in order to work properly
        if(strcmp(command, "help")==0){
            cputs("\nhere all commands are output inside in the kernel: \n\
help - all commands are output inside to the kernel\n");
        }
    }
}
