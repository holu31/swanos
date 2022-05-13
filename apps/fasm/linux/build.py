import os, shutil, sys

os.system(f"./compile {sys.argv[1]}")
os.system("i686-elf-gcc -nostdlib -T link.ld -o a.elf a.o")
os.system("rm a.o")