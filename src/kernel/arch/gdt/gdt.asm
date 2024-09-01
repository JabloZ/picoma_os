[bits 32]
global load_gdt
load_gdt:
    
   
  
    mov eax, [ebp+8]
    lgdt [eax]
    mov eax, [ebp+12]
  
    mov ax, [ebp+16]
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:reload_code_segment
    
reload_code_segment:
   
   

    ret
   
