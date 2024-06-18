
[org 0x7c00]
KERNEL_LOCATION equ 0x1000
mov [diskNum], dl
BITS 16
diskNum db 0
CODE_SEG equ code_descriptor - GDT_Start
DATA_SEG equ data_descriptor - GDT_Start
_main16: 
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov bp, 0x8000
    mov sp, bp

    mov bx, KERNEL_LOCATION

    ;reading the disk 
    mov ah, 2h
    mov al, 2
    mov ch, 0h
    mov cl, 2h
    mov dh, 0h
    mov dl, [diskNum]
    mov ah, 02h
    int 0x13

    jc raise_error_disk
    mov ah, 0x0
    mov al, 0x3
    int 0x10

    cli
    lgdt [GDT_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp CODE_SEG:start_protected_mode
    jmp $



raise_error_disk:
    mov si, diskErrorStr
    mov ah, 0x0e
    print_str_loop:
        mov al, [si]
        inc si
        cmp al, 0
        je end_
        int 0x10
        jmp print_str_loop


    

GDT_Start:
    null_decriptor: 
        dd 0
        dd 0
    code_descriptor:
        dw 0xffff
        dw 0
        db 0
        db 0b10011010
        db 0b11001111
        db 0
    data_descriptor:
        dw 0xffff
        dw 0
        db 0
        db 0b10010010
        db 0b11001111
        db 0


GDT_End:

GDT_descriptor: 
    dw GDT_End - GDT_Start - 1
    dd GDT_Start

[bits 32]
start_protected_mode:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ebp, 0x90000		; 32 bit stack base pointer
    mov esp, ebp
    jmp KERNEL_LOCATION

end_:

diskErrorStr: db "Couldn't read disk", 0
times 510-($-$$) db 0
dw 0xaa55


