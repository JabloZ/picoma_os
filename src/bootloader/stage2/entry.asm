
bits 16

section .entry

extern __bss_start
extern __end
extern _start
global entry
entry:
    cli
    mov ax, ds
    mov ss, ax
   
    ; setup stack at 0xFFF0
    mov sp, 0x9000
    mov bp, sp
    
    
    mov [bootDrive], dl
    
    
    call enable_a20
    lgdt [GDT_descriptor]
    mov eax, 0x10
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp 08h:protected_mode


protected_mode:
    [bits 32]

    
    ; 6 - setup segment registers
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
   
    mov edi, __bss_start
    mov ecx, __end
    sub ecx, edi
    mov al, 0
    cld
    rep stosb

    xor edx, edx
    mov dl, [bootDrive]
    push edx
    
    call _start

    cli
    hlt


enable_a20:
    [bits 16]
    
    call wait_input_a20
    mov al, kbDisableKeyboard
    out kbCommandPort, al

    call wait_input_a20
    mov al, kbReadOutPort
    out kbCommandPort, al

    call wait_output_a20
    in al, KbDataPort
    push eax

    call wait_input_a20
    mov al, kbWriteOutPort
    out kbCommandPort, al

    call wait_input_a20
    pop eax
    or al, 2
    out KbDataPort, al

    call wait_input_a20
    mov al, kbEnableKeyboard
    out kbCommandPort, al

    call wait_input_a20
    
    ret

wait_input_a20:
    in al, kbCommandPort
    test al, 2
    jnz wait_input_a20
    ret

wait_output_a20:
    in al, kbCommandPort
    test al, 1
    jz wait_output_a20
    ret
KbDataPort equ 60h
kbCommandPort equ 64h
kbDisableKeyboard equ 0xAD
kbEnableKeyboard equ 0xAE
kbReadOutPort equ 0xD0
kbWriteOutPort equ 0xD1

GDT_start:
    
    dd 0
    dd 0

    dw 0xFFFF
    dw 0
    db 0
    db 10011010b
    db 11001111b
    db 0

    dw 0xFFFF
    dw 0
    db 0
    db 10010010b
    db 11001111b
    db 0

    dw 0FFFFh                   
    dw 0                        
    db 0                       
    db 10011010b                
    db 00001111b                
    db 0                        

    dw 0FFFFh                  
    dw 0                        
    db 0                       
    db 10010010b                
    db 00001111b
    db 0              
GDT_descriptor:
    
    dw GDT_descriptor - GDT_start - 1
    dd GDT_start




bootDrive: dd 0