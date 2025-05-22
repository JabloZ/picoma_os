global jump_to_process
extern enter_usermode
jump_to_process:
    cli
    mov ax, 0x23
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    push 0x23
    push dword [ebp+8]
    push 0x202
    push 0x1b
    push dword [ebp+4]
    iretd
