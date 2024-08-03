[bits 32]

%macro isr_no_err 1
isr_%+%1:
    call isr_handle_routine
    iret 
%endmacro

%macro isr_err 1
isr_%+%1:
    call isr_handle_routine
    iret
%endmacro

extern exception_handler


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


global isr_table
isr_table:
%assign i 0 
%rep    32 
    dd isr_%+i ; use DQ instead if targeting 64-bit
%assign i i+1 
%endrep

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
