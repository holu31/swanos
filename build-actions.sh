#!/bin/bash

OBJECTS="bin/kernel.o bin/kernel_entry.o bin/console.o bin/ports.o bin/string.o bin/gdt.o bin/idt.o bin/interrupts.o bin/pci.o bin/isr.o bin/irq.o bin/keyboard.o bin/shell.o bin/description_tables.o"

i686-elf-gcc -g -I include -ffreestanding -Wall -Wextra -O0 -c src/io/ports.c -o bin/ports.o
i686-elf-gcc -g -I include -ffreestanding -Wall -Wextra -O0 -c src/io/console.c -o bin/console.o

i686-elf-gcc -g -I include -ffreestanding -Wall -Wextra -O0 -c src/libc/string.c -o bin/string.o

i686-elf-gcc -g -I include -ffreestanding -Wall -Wextra -O0 -c src/arch/x86/gdt.c -o bin/gdt.o
i686-elf-gcc -g -I include -ffreestanding -Wall -Wextra -O0 -c src/arch/x86/idt.c -o bin/idt.o
i686-elf-as src/arch/x86/description_tables.s -o bin/description_tables.o

i686-elf-gcc -g -I include -ffreestanding -Wall -Wextra -O0 -c src/interrupts/irq.c -o bin/irq.o
i686-elf-gcc -g -I include -ffreestanding -Wall -Wextra -O0 -c src/interrupts/isr.c -o bin/isr.o
nasm src/interrupts/interrupts.asm -f elf32 -O0 -o bin/interrupts.o

i686-elf-gcc -g -I include -ffreestanding -Wall -Wextra -O0 -c src/drivers/pci.c -o bin/pci.o
i686-elf-gcc -g -I include -ffreestanding -Wall -Wextra -O0 -c src/drivers/keyboard.c -o bin/keyboard.o
i686-elf-gcc -g -I include -ffreestanding -Wall -Wextra -O0 -c src/io/shell.c -o bin/shell.o

i686-elf-gcc -g -I include -ffreestanding -Wall -Wextra -O0 -c src/kernel.c -o bin/kernel.o

i686-elf-as src/kernel_entry.s -o bin/kernel_entry.o
i686-elf-gcc -g -I include -ffreestanding -Wall -Wextra -O0 -nostdlib -lgcc -T link.ld -o build/boot/kernel.elf $OBJECTS

grub-mkrescue -o "swanos-latest" build/ -V SwanOS
