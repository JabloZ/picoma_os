
global change_stack
change_stack:
    MOV   AX, 0x10 ; 0x10 is a stand-in for your data segment
    MOV   DS, AX
    MOV   ES, AX
    MOV   FS, AX
    MOV   GS, AX
    MOV   SS, AX
   
    mov esp, 0x600FF000
    mov ebp, esp
    ret
global reload_segments
reload_segments:
   ; Reload CS register containing code selector:
   JMP   0x08:.reload_CS ; 0x08 is a stand-in for your code segment
.reload_CS:
   ; Reload data segment registers:
   MOV   AX, 0x10 ; 0x10 is a stand-in for your data segment
   MOV   DS, AX
   MOV   ES, AX
   MOV   FS, AX
   MOV   GS, AX
   MOV   SS, AX
   RET