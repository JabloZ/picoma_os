
bits 16
%define ENDL 0x0D, 0x0A

mov [BOOT_DISK], dl

CODE_SEG equ GDT_code - GDT_start
DATA_SEG equ GDT_data - GDT_start

start:
    mov si, hello_world_str
    call print_str
    cli
    lgdt [GDT_start]
    mov eax, cr0
    or al, 1
    mov cr0, eax
    ;jmp CODE_SEG:start_protected_mode
.halt:
    cli
    hlt
print_str:
    
    print_str_loop:
        lodsb            
        or al, al
        je done
        mov ah, 0x0e
        int 0x10
        jmp print_str_loop
    done:
        ret
GDT_start:                          ; must be at the end of real mode code
    GDT_null:
        dd 0x0
        dd 0x0

    GDT_code:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10011010
        db 0b11001111
        db 0x0

    GDT_data:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10010010
        db 0b11001111
        db 0x0

GDT_end:

GDT_descriptor:
    dw GDT_end - GDT_start - 1
    dd GDT_start


[bits 32]
start_protected_mode:
    mov al, 'A'
    mov ah, 0x0f
    mov [0xb8000], ax
.halt
    cli
    hlt
BOOT_DISK: db 0
hello_world_str: db "Hello, world!", ENDL, 0