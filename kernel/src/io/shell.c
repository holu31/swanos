#include <kernel.h>
#include <libk/string.h>


char current_dir[256] = "/rd/apps/";


void ksh_main() {
    tty_setcolor(COLOR_ALERT);

    while (1) {
        tty_setcolor(COLOR_SYS_TEXT);
        tty_printf("\nroot ");
        tty_setcolor(COLOR_SYS_PATH);
        tty_printf("%s`# ", current_dir);

        tty_setcolor(COLOR_TEXT);

        char *cmd = keyboard_gets();

        if (strlen(cmd) == 0) {
            continue;
        } else if (strlen(cmd) > 256) {
            tty_setcolor(COLOR_ERROR);
            tty_printf("\nERROR: limit 256 char's!");
            continue;
        }

        tty_printf("\n");

        if (strcmp(cmd, "about") == 0) {
            tty_printf("SwanOS open and free x86 operating system written in C.");
        } else if (strcmp(cmd, "swanfetch") == 0){
            tty_setcolor(COLOR_SYS_TEXT);
            tty_printf("                    .~?YPPP5?~:\n");
            tty_printf("                  ?&@@@@@@@@@@@@G~\n");         
            tty_printf("                :&@@@#?^.. .:P@@@B5.\tOS: ");
            tty_setcolor(0xAAAAAA);
            tty_printf("SwanOS x86_32\n");
            tty_setcolor(COLOR_SYS_TEXT);              
            tty_printf("                &@@&:         Y@&?#5\tMemory: ");
            tty_setcolor(0xAAAAAA);   
            tty_printf("%d MiB / %d MiB\n", kheap_memory_used / (1024 * 1024), phys_installed_memory_size / (1024 * 1024));           
            tty_setcolor(COLOR_SYS_TEXT);
            tty_printf("               !@@@!            ^@@&\n");
            tty_printf("               ~@@@P             7@@\n");                        
            tty_printf("                B@@@#7.            :\n");              
            tty_printf("                 5@@@@@@#P?^.\n");                     
            tty_printf("                  .?B@@@@@@@@&BJ^\n");                 
            tty_printf("            ~Y7:      .!YB&@@@@@@&?\n");               
            tty_printf("          ^: .P@@#Y~.       :?G@@@@&.\n");             
            tty_printf("          .5&#5P@@@@@&BJ!:      5@@@B\n");             
            tty_printf("         ^5P#@@@@@@@@@@@@@@&P^   &@@@\n");             
            tty_printf("           ?&@@@@@@@@@@@@@@@@@#: &@@&\n");             
            tty_printf("             .J&@@@@@@@@@@@@@@@@#@@@~\n");             
            tty_printf("                G@@@@@@@@@@@@@@@@@@~\n");              
            tty_printf("                 .Y&@@@@@@@@@@@@#?\n");                
            tty_printf("                    .:!?Y5P5Y7^.\n");
        } else if (strcmp(cmd, "help") == 0) {
            tty_printf("Commands:\n" \
                        "help                   get list of commands\n" \
                        "cat   <filename>       open file to read\n" \
                        "cd    <folder>         open folder\n" \
                        "./<filename>           run .elf programm in current folder\n" \
                        "ls                     print list of files\n" \
                        "swanfetch              get system info" 
                        );
        } else if (strlen(cmd) > 4 && strncmp(cmd, "cat ", 4) == 0) {
            char fname[256];

            char *tok = (char *)strtok(cmd, " ");
            
            tok = (char *)strtok(0, " "); // tok - now is filename

            if (fname != 0) {
                cat(tok);
            } else {
                tty_setcolor(COLOR_ERROR);
                tty_printf("cat: incorrect argument\n");
            }
        } else if (strlen(cmd) > 3 && strncmp(cmd, "cd ", 3) == 0) {
            char dname[256];
            
            char *tok = (char *)strtok(cmd, " ");
            
            tok = (char *)strtok(0, " "); // tok - now is dirname

            if (dname != 0) {
                cd(tok);
            } else {
                tty_setcolor(COLOR_ERROR);
                tty_printf("\ncd: incorrect argument\n");
            }
        } else if (strcmp(cmd, "ls") == 0) {
            initrd_list(0, 0);
        } else if (strlen(cmd) > 2 && strncmp(cmd, "./", 2) == 0) {
            char fname[256];

            char *tok = (char *)strtok(cmd, "/");

            tok = (char *)strtok(0, "/"); // tok - now is filename

            if (fname != 0) {
                char temp[256] = {0};
                strcpy(temp, current_dir);
                strcat(temp, tok);
                run_elf_file(temp);

            } else {
                tty_setcolor(COLOR_ERROR);
                tty_printf("run: incorrect argument\n");
            }
        } else {
            tty_setcolor(COLOR_ERROR);
            tty_printf("%s: unknown cmd\n", cmd);
        }
    }
}


void cd(char *dname) {
    if (dname[0] != '/') {
        char temp[256];

        strcpy(temp, current_dir);

        temp[strlen(temp) - 1] = 0;

        strcat(temp, dname);

        temp[strlen(temp) - 1] = 0;
        temp[strlen(temp) - 1] = 0;

        strcpy(dname, temp);
    }


    if (dname[strlen(dname) - 1] != '/') {
        strcat(dname, "/");
    }

    if (vfs_exists(dname) && vfs_is_dir(dname)) {
        strcpy(current_dir, dname);
    } else {
        tty_setcolor(COLOR_ERROR);
        tty_printf("cd: no such directory\n");
    }
}


void cat(char *fname) {
    if (fname[0] != '/') {
        char temp[256];

        strcpy(temp, current_dir);
        temp[strlen(temp) - 1] = 0;

        strcat(temp, fname);

        temp[strlen(temp) - 1] = 0;
        temp[strlen(temp) - 1] = 0;

        strcpy(fname, temp);
    }

    char *buf = (char*) kheap_malloc(2048);

    if (!vfs_exists(fname)) {
        tty_setcolor(COLOR_ERROR);
        tty_printf("cat: error file not found\n");
    } else {
        uint32_t fsize = vfs_get_size(fname);
        int res = vfs_read(fname, 0, fsize, buf);
        (void)res;

        buf[fsize] = '\0';

        tty_printf("%s:\n\n%s\n", fname, buf);

        kheap_free(buf);
    }
}