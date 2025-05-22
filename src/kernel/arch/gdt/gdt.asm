[bits 32]
global load_gdt

load_gdt:
    push ebp
    mov ebp, esp

    mov eax, [ebp+8]
    lgdt [eax]
    
    mov eax, [ebp+12]
    push eax
    push .reload_code_segment
    retf

.reload_code_segment:
    mov ax, [ebp+16]
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, ebp
    pop ebp
   

    ret
   
global flush_tss
flush_tss:
	mov ax, (5 * 8) | 0
	ltr ax
	ret