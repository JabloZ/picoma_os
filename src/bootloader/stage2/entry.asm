
bits 16

section .text
extern _start
global entry
entry:
    mov ah, 0x0e
    mov al, 'a'
    int 10h

    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov ss, ax
    ; setup stack at 0xFFF0
    mov sp, 0xFFFF
    mov bp, sp
    ; expect boot drive in dl, send it as argument to cstart function
    mov [bootDrive], dl
    
    cli
    ;call enable_a20
    cli
    lgdt [GDT_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    
    jmp 08h:protected_mode
    jmp $

protected_mode:
    [bits 32]
    mov eax, 10h
    mov ds, eax
    mov es, eax
    mov fs, eax
    mov gs, eax
    mov ss, eax
    mov esp, 0x9000
    ;push [bootDrive]
    call _start

    
    cli
    hlt  
    jmp real_mode

;protected_mode_16:

;real_mode:

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

GDT_start:
    
    dd 0
    dd 0

GDT_code equ $-GDT_start:
    dw 0xFFFF
    dw 0
    db 0
    db 10011010b
    db 11001111b
    db 0

GDT_data equ $-GDT_start:
    dw 0xFFFF
    dw 0
    db 0
    db 10010010b
    db 11001111b
    db 0
GDT_code_16 equ $-GDT_start:
    dw 0FFFFh                   
    dw 0                        
    db 0                       
    db 10011010b                
    db 00001111b                
    db 0                        

GDT_data_16 equ $-GDT_start:
    dw 0FFFFh                  
    dw 0                        
    db 0                       
    db 10010010b                
    db 00001111b
    db 0              
GDT_descriptor:
    
    dw GDT_descriptor - GDT_start - 1
    dd GDT_start

CODE_SEG equ GDT_code - GDT_start
DATA_SEG equ GDT_data - GDT_start



section .data
KbDataPort equ 60h
kbCommandPort equ 64h
kbDisableKeyboard equ 0xAD
kbEnableKeyboard equ 0xAE
kbReadOutPort equ 0xD0
kbWriteOutPort equ 0xD1
bootDrive dd 0