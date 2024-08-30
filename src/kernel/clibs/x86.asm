
global outb
outb:
    [bits 32]
    
    mov dx, [esp + 4]
    mov al, [esp + 8]
    out dx, al
    
    ret
global inb
inb:
    [bits 32]
    mov dx, [esp+4]
    in al, dx
    ret
extern linear_to_real

%macro enter_real_mode 0
    [bits 32]
    jmp dword 18h:.protected_16        ; 1 - jump to 16-bit protected mode segment

.protected_16:
    [bits 16]
    ; 2 - disable protected mode bit in cr0
    mov eax, cr0
    and al, ~1
    mov cr0, eax

    jmp dword 00h:.real_mode
.real_mode:
    [bits 16]
    mov ax, 0
    mov ds, ax
    mov ss, ax
    sti
%endmacro

%macro offset 4

    mov %3, %1      ; linear address to eax
    shr %3, 4
    mov %2, %4
    mov %3, %1      ; linear address to eax
    and %3, 0x0f

%endmacro

%macro enter_protected_mode 0
    cli
    mov eax, cr0 
    or al, 1
    mov cr0, eax
    jmp dword 08h:.protected_mode
.protected_mode:
    [bits 32]
    mov ax, 10h
    mov ds, ax
    mov ss, ax
%endmacro


global x86_reset_disk
x86_reset_disk:
    
    
    push ebp
    mov ebp, esp
    
    pushad
    pushfd
    
    enter_real_mode
    mov ah, 0
    mov dl, byte [ebp+8] ;drive, uint8
    
    mov ah, 0x00
    int 13h

    mov eax, 1
    sbb eax, 0; 1 on true (success) 

    push eax
    enter_protected_mode
    pop eax
    popfd
    popad
    mov esp, ebp
    pop ebp
    ret
global x86_read_disk
x86_read_disk:
    push ebp
    mov ebp, esp
    enter_real_mode
    push ebx
    push es

    mov dl, [bp+8]; drive
    
    mov ch, [bp+12]; cylinder
    mov cl, [bp+13];
    shl cl, 6

    mov al, [bp+16]; sector
    and al, 3Fh
    or cl, al

    mov dh, [bp+20];

    mov al, [bp+24]; ptr data out
    
    offset [bp + 28], es, ebx, bx
    
    mov ah, 02h
    stc
    int 13h
    mov eax, 1
    sbb eax, 0 ; 1 on success

    pop es
    pop ebx

    push eax
    enter_protected_mode
    pop eax

    mov esp, ebp
    pop ebp
    ret


global x86_disk_parameters
x86_disk_parameters: ; 624
    [bits 32]
    
    push ebp
    mov ebp, esp
    
    enter_real_mode
    [bits 16]
    push es
    push bx
    push esi
    push di
    
    mov dl, [bp+8] ; disk
    mov ah, 08h
    mov di, 0
    mov es, di
    stc 
    int 13h

    mov eax, 1
    sbb eax, 0

    ;type
    offset [bp+12], es, esi, si ;type
    mov [es:si], bl
    
    ;cylinders
    mov bl, ch
    mov bh, cl
    shr bh, 6
    inc bx
    
    offset [bp+16], es, esi, si; cylinders
    mov [es:si], bx

    ;sectors
    xor ch, ch 
    and cl, 3Fh
    offset [bp+20], es, esi, si ;sectors
    mov [es:si], cx

    ; heads 
    mov cl, dh
    inc cx
    offset [bp+24], es, esi, si ; heads
    mov [es:si], cx

    pop di
    pop esi
    pop bx
    pop es
    push eax
    enter_protected_mode
    [bits 32]
    pop eax

    mov esp, ebp
    pop ebp
    ret

