#include <shell.h>
#include <keyboard.h>
#include <console.h>
#include <ports.h>
#include <string.h>
#include <kernel.h>
#include <pci.h>
#include <pcspkr.h>

#define SIZE_BUFFER 77

char buffer_shell[SIZE_BUFFER];

char *get_command(){
    int key = kb_getkey();
    cputs("\n");
    cputs(KERNEL_NAME);
    cset_color(COLOR_CYAN, COLOR_BLACK);
    cputs(" ~$ ");
    cset_color(COLOR_LIGHT_GREY, COLOR_BLACK);
    memset(buffer_shell, 0, SIZE_BUFFER); // clear buffer
    while(kb_sctochar(key) != '\n'){
        if(strlen(buffer_shell) == SIZE_BUFFER){ // checking if the buffer overflows...
            log("buffer full\n", false);         // ...temporary until we write a memory manager
            return "";
        }
        if(kb_sctochar(key) == '\r'){
            if(strlen(buffer_shell) != 0){
                buffer_shell[strlen(buffer_shell)-1] = '\0';
                cbackspace();
            }
        }
        else if(kb_sctochar(key) != 0){ 
            buffer_shell[strlen(buffer_shell)] = kb_sctochar(key);
            cputch(kb_sctochar(key));
        }
        key = kb_getkey();
    }
    return buffer_shell;
}

void shell_init(){
    cset_color(COLOR_CYAN, COLOR_BLACK);
    cputs("\n\
   _____                     ____   _____ \n\
  / ____|                   / __ \\ / ____|\n\
 | (_____      ____ _ _ __ | |  | | (___  \n\
  \\___ \\ \\ /\\ / / _` | '_ \\| |  | |\\___ \\ \n\
  ____) \\ V  V / (_| | | | | |__| |____) |\n\
 |_____/ \\_/\\_/ \\__,_|_| |_|\\____/|_____/ \n\
");
    cset_color(COLOR_LIGHT_GREY, COLOR_BLACK);
    for(;;){
        char *command = get_command();
        char *arg = strtok(command, " ");
        if(strcmp(command, "")!=0){
            if(strcmp(arg, "help")==0){
                cputs("\nhere all commands are output inside in the kernel: \n\n\
              help | all commands are output inside to the kernel\n\
-------------------|---------------------------------------------\n\
            reboot | reboot the computer\n\
          shutdown | shutdown the computer\n\
-------------------|---------------------------------------------\n\
       cls / clear | clear entire screen\n\
     ver / version | show os version\n\
             lspci | list devices on pci bus\n\
              beep | beep with pc speaker\n\
");
            } else if(strcmp(arg, "reboot")==0){
                uint8_t good = 0x02;
                while(good & 0x02) good = inb(0x64);
                outb(0x64, 0xFE);
                asm volatile("hlt");
            } else if(strcmp(arg, "shutdown")==0){
                outw(0xB004, 0x2000);
                outw(0x604, 0x2000);
                outw(0x4004, 0x3400);
            } else if(strcmp(arg, "cls")==0 || strcmp(arg, "clear")==0){
                cinit();
            } else if(strcmp(arg, "ver")==0 || strcmp(arg, "version")==0){
                cputs("\n");
                cputs(KERNEL_NAME);
                cputs(" ");
                cputs(KERNEL_VERSION_STRING);
                cputs(" ");
                cputs(__DATE__);
                cputs(" ");
                cputs(__TIME__);
                cputs("\n");
            } else if(strcmp(arg, "lspci")==0){
                cputs("\nList of PCI devices:\n");
                pci_getbuses();
            } else if(strcmp(arg, "beep")==0){
                beep(1000, 200);
            } else{
                cputs("\ncommand ");
                cputs(arg);
                cputs(" not found");
            }
        }
    }
}
