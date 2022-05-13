use32

    db 0x0
    db 0x1
    db 'SEF'

entry_point:
    mov eax, 0
    mov ebx, hw
    int 80h
    ret

hw db 'Hello world', 0
    