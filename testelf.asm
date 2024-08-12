section .text
global _start

_start:
    mov eax, 5    ; Argument 1
    mov ebx, 10   ; Argument 2
    call add_numbers
    ; eax now contains the sum of the two arguments
    mov ebx, eax      ; przekazanie kodu wyjścia
    mov eax, 1        ; kod systemowy do zakończenia programu (sys_exit)
    int 0x80   
add_numbers:
    add eax, ebx  ; Add the two arguments
    ret