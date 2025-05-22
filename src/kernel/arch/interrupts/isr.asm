[bits 32]
extern exception_handler

%macro isr_no_err 1
global isr_%1:
isr_%1:
    push 0
    push %1
    jmp isr_handle_routine
%endmacro

%macro isr_err 1
global isr_%1:
isr_%1:
    push %1
    jmp isr_handle_routine
%endmacro


isr_no_err 0
isr_no_err 1
isr_no_err 2
isr_no_err 3
isr_no_err 4
isr_no_err 5
isr_no_err 6
isr_no_err 7
isr_err 8
isr_no_err 9
isr_err 10
isr_err 11
isr_err 12
isr_err 13
isr_err 14
isr_no_err 15
isr_no_err 16
isr_err 17
isr_no_err 18
isr_no_err 19
isr_no_err 20
isr_no_err 21
isr_no_err 22
isr_no_err 23
isr_no_err 24
isr_no_err 25
isr_no_err 26
isr_no_err 27
isr_no_err 28
isr_no_err 29
isr_err 30
isr_no_err 31
isr_no_err 32
isr_no_err 33
isr_no_err 34
isr_no_err 35
isr_no_err 36
isr_no_err 37
isr_no_err 38
isr_no_err 39
isr_no_err 40
isr_no_err 41
isr_no_err 42
isr_no_err 43
isr_no_err 44
isr_no_err 45
isr_no_err 46
isr_no_err 47
isr_no_err 48




isr_handle_routine:
    pusha               
    xor eax, eax        
    mov ax, ds
    push eax

    mov ax, 0x10        
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    push esp
    call exception_handler
    add esp, 4

    pop eax             
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa                
    add esp, 8         
    iret               
global panic
panic:
    cli
    hlt
global isr6
extern fdc_irq_handle
isr6:
    cli
    pusha
    call fdc_irq_handle
    popa
    sti
    iretd
global isr80
extern syscall_handler
isr80:
    cli
    
    push ds
   

    push esi
    push edi
    push ebp
    push dword 0
    push ebx
    push edx
    push ecx
    push eax

    
    mov eax, esp
    push eax
    call syscall_handler
    add esp, 4
   
    pop eax
    pop ecx
    pop edx
    pop ebx
    add esp, 4
    pop ebp
    pop esi
    pop edi

    pop ds
    
    sti
    iretd