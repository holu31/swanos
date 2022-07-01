#include <shell.h>
#include <keyboard.h>
#include <console.h>
#include <ports.h>
#include <string.h>
#include <kernel.h>
#include <pci.h>
#include <pcspkr.h>
#include <stdlib.h>
#include <cmos.h>

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
                cprintf("\nhere all commands are output inside in the kernel: \n\n\
              help | all commands are output inside to the kernel\n\
-------------------|---------------------------------------------\n\
            reboot | reboot the computer\n\
          shutdown | shutdown the computer\n\
-------------------|---------------------------------------------\n\
       cls / clear | clear entire screen\n\
     ver / version | show os version\n\
             lspci | list devices on pci bus\n\
              beep | beep with pc speaker\n\
       time / date | current date and time\n\
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
                cprintf("\n%s %s %s %s", KERNEL_NAME, KERNEL_VERSION_STRING, __DATE__, __TIME__);
            } else if(strcmp(arg, "lspci")==0){
                cputs("\nList of PCI devices:\n");
                pci_getbuses();
            } else if(strcmp(arg, "beep")==0){
                beep(1000, 200);
            } else if(strcmp(arg, "time")==0 || strcmp(arg, "date")==0){
                char c_second[32];
                char c_minute[32];
                char c_hour[32];
                char c_day[32];
                char c_year[32];

                char* months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

                int32_t second = cmos_get_second();
                int32_t minute = cmos_get_minute();
                int32_t hour = cmos_get_hour();
                int32_t day = cmos_get_day();
                int32_t month = cmos_get_month();
                int32_t year = cmos_get_year();

                itoa(second, c_second);
                itoa(minute, c_minute);
                itoa(hour, c_hour);
                itoa(day, c_day);
                itoa(year, c_year);

                cputs("\n\n   Current time: ");

                if (hour < 10) {
                    cputs("0");
                }
                cputs(c_hour);
                cputs(":");
                if (minute < 10) {
                    cputs("0");
                }
                cputs(c_minute);
                cputs(":");
                if (second < 10) {
                    cputs("0");
                }
                cputs(c_second);

                cputs("\n   Current date: ");

                if (day < 10) {
                    cputs("0");
                }
                cputs(c_day);
                cputs(" ");
                cputs(months[month - 1]);
                cputs(" ");
                cputs(c_year);
                cputs("\n");
            } else{
                cputs("\ncommand ");
                cputs(arg);
                cputs(" not found");
            }
        }
    }
}
