
[org 0x7c00]
BITS 16
diskNum db 0

start: 
    mov [diskNum], dl
    xor ax, ax
    mov ds, ax
    mov es, ax

    mov ah, 2
    mov al, 1
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, [diskNum]
    mov bx, 0x7E00
    int 0x13

    mov dl, 0
    mov ah, 0x15
    int 0x13

    cmp al, 0
    je print_str
    ;jmp $
print_str:
    mov ah, 0x0e
    mov si, diskErrorStr
    
    print_str_loop:
        mov al, [si]
        inc si
        cmp al, 0
        je end_
        int 0x10
        jmp print_str_loop
    
end_:
    jmp $
diskErrorStr: db "Couldn't read disk", 0
times 510-($-$$) db 0
dw 0xaa55


