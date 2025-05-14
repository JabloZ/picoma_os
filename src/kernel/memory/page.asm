
global reload_segments
global switch_current_dir
global enable_paging_flag
global set_stack_top_4mb
global reset_stack

reset_stack:
   mov ax, 0x10
   mov ss, ax
   mov esp, 0xFFFF
   sti
   ret
reload_segments:
   
   JMP   0x08:.reload_CS 
.reload_CS:
 
   MOV   AX, 0x10 
   MOV   DS, AX
   MOV   ES, AX
   MOV   FS, AX
   MOV   GS, AX
   MOV   SS, AX
   RET
switch_current_dir:
   mov eax, [esp+4]
   mov cr3, eax
   ret
enable_paging_flag:
   mov eax, cr0
   or eax, 0x80000000
   mov cr0, eax
   ret

