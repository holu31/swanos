.global enable_paging

enable_paging:
    # move to CR3
    mov 4(%esp), %eax
    #mov (0x00009000), %eax
    mov %eax, %cr3

    mov %cr4, %ecx
    and $~0x00000010, %ecx
    mov %ecx, %cr4

    # enable paging
    mov %cr0, %eax
    or $0x80000000, %eax
    mov %eax, %cr0
    ret
