

[BITS 32]

%macro enter_protected_mode 0
    cli
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp dword 08h:.protected_mode
.protected_mode:
    [BITS 32]
    mov ax, 10h
    mov ds, ax
    mov ss, ax
%endmacro
%define KERNEL_VIRTUAL_ADDRESS 0xC0000000
%define PAGE_SIZE 4096
%define STACK_SIZE 16384
%define KERNEL_LOAD_ADDRESS 0x00100000      

%macro regs 0
    jmp 0x08:.flush_segments
.flush_segments:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
%endmacro

section .bss
stack_bottom:
    resb 4096
stack_top:


section .prekernel.bss
    align 4096 
    
boot_page_directory:
    resb 4096
boot_page_table1:
    resb 4096
    
global prepare_paging
prepare_paging:
    cld
    ;enter_protected_mode
    ;regs
    
    mov edi, boot_page_table1
   
    mov esi, 0x000000

   mov ecx, 1024 


.map_kernel:
    ;cmp esi, _kernel_end-0xc0000000
    ;jae after_map
    mov edx, esi
    or edx, 0x003           
    mov [edi], edx
    add esi, 4096
    add edi, 4
    loop .map_kernel
    jmp after_map
after_map:
    ;mov dword [boot_page_table1 + (1023 * 4)], (0x000B8000 | 0x003)

 
    mov eax, boot_page_table1
    or eax, 0x003
    mov [boot_page_directory+ (768 * 4)], eax
    mov [boot_page_directory], eax

    mov ecx, (boot_page_directory)
    mov cr3, ecx
     
    
    mov ecx, cr0
    or ecx, 0x80000001       
    mov cr0, ecx
   
    
    jmp upper_mem
    
upper_mem:
   
    mov eax, KERNEL_VIRTUAL_ADDRESS
    add esp, eax  
    mov ebp, esp           
    
    lgdt [gdt_descriptor]
    
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    jmp 0x08:reload_cs
    jmp reload_cs
    
reload_cs:
  
    ;mov eax, 0
    ;mov [boot_page_directory], eax
    
    ret

global change_stack
change_stack:
    mov eax, 0xc0000000
    add esp, eax
    ;mov ebp, esp
    ret
global unmap
unmap:
    mov eax, 0
    mov [boot_page_directory], eax
    jmp upper_memory_reg+0xc0000000
upper_memory_reg:

    jmp 0
    ret

gdt_start:
    dq 0x0000000000000000  ; Null segment

    ; Segment kodu: base=0x0, limit=4GB, type=0x9A (execute/read, non-conforming, code segment)
    dq 0x00CF9A000000FFFF  ; Flags: G=1, D/B=1, L=0, AVL=0

    ; Segment danych: base=0x0, limit=4GB, type=0x92 (read/write, data segment)
    dq 0x00CF92000000FFFF  ; Flags: G=1, D/B=1, L=0, AVL=0

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1   ; Limit (w bajtach)
    dd gdt_start+0xc0000000  