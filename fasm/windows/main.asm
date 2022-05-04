format ELF

public main

main:
    mov eax, 0
    mov ebx, hello
    int 80h
    ret

hello db 'Hello world!', 0
