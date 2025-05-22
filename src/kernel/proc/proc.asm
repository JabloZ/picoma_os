global jump_to_process
jump_to_process:
    cli
    mov eax, [esp+4]
    mov ebx, [esp+8]

    mov ax, 0x23
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push 0x23       
    push ebx        
    push 0x200     
    push 0x1B        
    push eax        
    iretd
