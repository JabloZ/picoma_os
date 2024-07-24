%macro enter_real_mode 0
    
    mov eax, cr0 
    or al, ~1
    mov cr0, eax
    jmp dword 00h:.real_mode
real_mode:
    [bits 16]
    mov ax, 0
    mov ds, ax
    mov ss, ax
    sti
%endmacro


%macro enter_protected_mode 0
    cli
    mov eax, cr0 
    or al, 1
    mov cr0, eax
    jmp dword 08h:.protected_mode
protected_mode:
    [bits 32]
    mov ax, 10h
    mov ds, ax
    mov ss, ax
%endmacro

global outb
outb:
    [bits 32]
    mov dx, [esp + 4]
    mov al, [esp + 8]
    out dx, al
    ret

global x86_reset_disk
x86_reset_disk:
    [bits 32]
    push ebp
    mov ebp, esp
    
    mov dl, [bp+8] ;drive, uint8
    stc
    int 13h

    mov eax, 1
    sbb eax, 0; 1 on true (success) 
    push eax
global x86_read_disk
x86_read_disk:
    push bp
    mov bp, sp
    mov dl, [bp+4]; drive
    
    mov ch, [bp+6]; cylinder
    mov cl, [bp+7];
    shl cl, 6

    mov dh, [bp+8]; head

    mov al, [bp+10]; sectors
    and al, 3Fh
    or cl, al

    mov al, [bp+12]; al-count

    mov bx, [bp+16]; ptr data out
    mov es, bx
    mov bx, [bp+14]
    
    mov al, 02h
    stc
    int 13h
    mov ax, 1
    sbb ax, 0 ; 1 on success

    pop es
    pop bx

    mov sp, bp
    pop bp
    ret
global x86_disk_parameters
x86_disk_parameters:
    push bp
    mov bp, sp

    push es
    push bx
    push si
    push di

    mov dl, [bp+4]
    mov ah, 08h
    mov di, 0
    mov es, di
    stc 
    int 13h

    mov ax, 1
    sbb ax, 0

    mov si, [bp+6];drive type
    mov [si], bl

    mov bl, ch
    mov bh, cl
    shr bh, 6
    mov si, [bp+8]; cylinders

    xor ch, ch ;sectors
    and cl, 3Fh
    mov si, [bp+10]
    mov [si], cx

    mov cl, dh; heads
    mov si, [bp+12]
    mov [si], cx

    pop di
    pop si
    pop bx
    pop es

    mov sp, bp
    pop bp
    ret