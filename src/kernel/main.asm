
bits 16
%define ENDL 0x0D, 0x0A

start:
    mov si, hello_world_str
    call print_str
   
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

BOOT_DISK: db 0
hello_world_str: db "Hello(kernel)!", ENDL, 0