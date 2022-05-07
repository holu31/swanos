import os, shutil, sys

os.system(f"i686-elf-gcc -nostdlib -w -lgcc -ffreestanding -I include/ -c libc/stdio.c -o bin/stdio.o")

os.system(f"i686-elf-gcc -nostdlib -w -lgcc -ffreestanding -I include/ -c {sys.argv[1]} -o a.o")
os.system("i686-elf-gcc -nostdlib -T link.ld -o a.elf a.o bin/stdio.o")
os.system("rm a.o")