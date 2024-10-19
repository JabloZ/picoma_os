org 0x7C00
[bits 16]
%define ENDL 0x0D, 0x0A


jmp short start
nop
start:
    xor ax, ax
    mov ds, ax
    mov es, ax
    
    mov ss, ax
    mov sp, 0x7C00
   
    mov [drive_number], dl
    .after:
        push es
        mov ah, 08h
        int 13h
        jc disk_error   

       mov bx, stage2_load_segment
        mov es, bx
        mov bx, stage2_load_offset
        
        mov ah, 02
        mov al, 1 ;sectors to read
        mov ch, 0 ;cylinder
        mov cl, 1 ;sector
        mov dh, 1 ;head
        mov dl, [drive_number]
        mov ah, 02h
        
        
        
    .load_stage_2:
        mov ah, 02h
        mov al, 1
        
        int 13h
        jc disk_error

        
       add bx, 512
        cmp cl, 18
        je .change_head 
        jmp .end
    .change_head:
        cmp dh, 0
        je .change_head_to_0
        mov dh, 1
        mov cl, 0
        jmp .end
    .change_head_to_0:
        mov dh, 0
        add ch, 1
        mov cl, 0
    .end:

    
        ;add bx, 512
        add cl, 1
         
        add word [sectors_read], 1
        cmp word [sectors_read], 50
        
        jge .reading_finished
        jmp .load_stage_2
        
    .reading_finished:
       
        mov dl, 0
        mov si, 0
        mov ax, stage2_load_segment
        mov ds, ax
        mov es, ax
        
        ;cli
        ;hlt;7c74
        jmp stage2_load_segment:stage2_load_offset
        ;mov ah, 0x0e
        ;mov al, bl
        ;int 0x10
        cli
        hlt




disk_error:
    mov si, disk_err
    call print_str
    jmp wait_and_reboot
    hlt



print_str:
    push si
    push ax
    push bx
    .print_str_loop:
        lodsb               ; loads next character in al
        or al, al
        jz done
        
        MOV ah, 0x0E
        MOV bh, 0
        INT 0x10
        jmp .print_str_loop
done:
     popa

    pop di
    pop dx
    pop cx
    pop bx
    pop ax
    ret        

wait_and_reboot:
    mov ah, 0
    int 16h
    jmp 0FFFFh:0
disk_success:   db 'bios', ENDL, 0
disk_err: db 'disk error', ENDL, 0
stage2_msg: db 'TTried stage 2', ENDL, 0

stage2_sector: dw 0
drive_number: db 0
sectors_to_read equ 80
sectors_read: dw 0
stage2_file:    db 'STAGE2  BIN'
stage2_load_segment equ 0
stage2_load_offset equ 0x1000
current_sector equ 0
times 510-($-$$) db 0
dw 0xAA55

buffer:
