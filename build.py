#!/usr/bin/env python3
import os, sys

OBJECTS = "bin/kernel.o bin/kernel_entry.o bin/console.o bin/ports.o bin/string.o bin/gdt.o bin/idt.o bin/interrupts.o bin/pci.o bin/isr.o bin/irq.o bin/keyboard.o bin/shell.o bin/description_tables.o"

os.system('i686-elf-gcc -g -I include -ffreestanding -Wall -Wextra -O0 -c src/io/ports.c -o bin/ports.o')
os.system('i686-elf-gcc -g -I include -ffreestanding -Wall -Wextra -O0 -c src/io/console.c -o bin/console.o')

os.system('i686-elf-gcc -g -I include -ffreestanding -Wall -Wextra -O0 -c src/libc/string.c -o bin/string.o')

os.system('i686-elf-gcc -g -I include -ffreestanding -Wall -Wextra -O0 -c src/arch/x86/gdt.c -o bin/gdt.o')
os.system('i686-elf-gcc -g -I include -ffreestanding -Wall -Wextra -O0 -c src/arch/x86/idt.c -o bin/idt.o')
os.system('i686-elf-as src/arch/x86/description_tables.s -o bin/description_tables.o')

os.system('i686-elf-gcc -g -I include -ffreestanding -Wall -Wextra -O0 -c src/interrupts/irq.c -o bin/irq.o')
os.system('i686-elf-gcc -g -I include -ffreestanding -Wall -Wextra -O0 -c src/interrupts/isr.c -o bin/isr.o')
os.system('nasm src/interrupts/interrupts.asm -f elf32 -O0 -o bin/interrupts.o')

os.system('i686-elf-gcc -g -I include -ffreestanding -Wall -Wextra -O0 -c src/drivers/pci.c -o bin/pci.o')
os.system('i686-elf-gcc -g -I include -ffreestanding -Wall -Wextra -O0 -c src/drivers/keyboard.c -o bin/keyboard.o')
os.system('i686-elf-gcc -g -I include -ffreestanding -Wall -Wextra -O0 -c src/io/shell.c -o bin/shell.o')

os.system('i686-elf-gcc -g -I include -ffreestanding -Wall -Wextra -O0 -c src/kernel.c -o bin/kernel.o')

os.system('i686-elf-as src/kernel_entry.s -o bin/kernel_entry.o')
os.system('i686-elf-gcc -g -I include -ffreestanding -Wall -Wextra -O0 -nostdlib -lgcc -T link.ld -o build/boot/kernel.elf {}'.format(OBJECTS))
if sys.platform == "linux" or sys.platform == "linux2":
    os.system("""grub-mkrescue -o "swanos-latest.iso" build/ -V SwanOS""")
else:
    os.system("""wsl grub-mkrescue -o "swanos-latest.iso" build/ -V SwanOS""")

os.system("qemu-system-i386 -m 16 -name SwanOS -cdrom swanos-latest.iso -serial file:Qemu.log")
