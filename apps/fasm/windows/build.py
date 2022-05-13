import os, shutil, sys

os.system(f"compile.exe {sys.argv[1]} a.o")
os.system("i686-elf-gcc -nostdlib -T link.ld -o a.elf a.o")
os.system("del a.o")