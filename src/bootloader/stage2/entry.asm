
%define ENDL 0x0D, 0x0A
mov ah, 0x0e
    mov al, 'A'
    int 10h
bits 16
section .entry

extern _start

extern __bss_start
extern __end

global entry
entry:
    mov ah, 0x0e
    mov al, 'A'
    int 10h
    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; setup stack at 0xFFF0
    mov esp, 0xFFF0
    mov ebp, esp
    sti

    ; expect boot drive in dl, send it as argument to cstart function
    mov [bootDrive], dl
    

    cli 
    call enable_a20
    call GDT_load
    mov eax, cr0
    or al, 1
    mov cr0, eax
    ;jmp CODE_SEG:protected_mode
   
    jmp dword 08h:.protected_mode
    ;jmp dword:protected_mode
.protected_mode:
    
    [bits 32]

    mov ax, 10h
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov al, 0
    cld
    rep stosb


    push dword[bootDrive]
    call _start
    
    cli
    hlt



enable_a20:
    cli
    
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
    sti
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

GDT_descriptor:
    dw GDT_descriptor - GDT_start - 1
    dd GDT_start

GDT_load:
    lgdt [GDT_descriptor]
    ret
.halt:
    jmp .halt


    
KbDataPort equ 60h
kbCommandPort equ 64h
kbDisableKeyboard equ 0xAD
kbEnableKeyboard equ 0xAE
kbReadOutPort equ 0xD0
kbWriteOutPort equ 0xD1
bootDrive dd 0