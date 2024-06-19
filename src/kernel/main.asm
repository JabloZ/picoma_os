org 0x0
bits 16
%define ENDL, 0x0D, 0x0A

start:
    mov si, hello_world_str
    call print_str
.halt:
    cli
    hlt
print_str:
    
    mov ah, 0x0e
    print_str_loop:
        mov al, [si]
        inc si
        cmp al, 0
        je end_
        int 0x10
        jmp print_str_loop
end_:


hello_world_str: db "Hello, world!", 0