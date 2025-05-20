global enter_usermode
enter_usermode:
    cli
    mov ax, 0x23
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    push 0x23
    push esp
    pushfd
    push 0x1b
    lea eax, [a]
    push eax
    
    iretd
a:
    add esp, 4
global flush_tss
flush_tss:
	mov ax, (5 * 8) | 0
	ltr ax
	ret