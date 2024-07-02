org 0x7C00
bits 16
%define ENDL 0x0D, 0x0A



jmp short start
nop

;set up fat12
bdb_oem:                    db 'MSWIN4.1'           
bdb_bytes_per_sector:       dw 512
bdb_sectors_per_cluster:    db 1
bdb_reserved_sectors:       dw 1
bdb_fat_count:              db 2
bdb_dir_entries_count:      dw 0E0h
bdb_total_sectors:          dw 2880                 
bdb_media_descriptor_type:  db 0F0h                 
bdb_sectors_per_fat:        dw 9                    
bdb_sectors_per_track:      dw 18
bdb_heads:                  dw 2
bdb_hidden_sectors:         dd 0
bdb_large_sector_count:     dd 0

ebr_drive_number:           db 0                    
                            db 0                   
ebr_signature:              db 29h
ebr_volume_id:              db 12h, 34h, 56h, 78h   
ebr_volume_label:           db 'PICOMA OS  '        
ebr_system_id:              db 'FAT12   '

start:
    ;data segments
    
    mov ax, 0   
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    
    .after_start:
        
        mov ax, [bdb_sectors_per_fat]
        mov bl, [bdb_fat_count]
        xor bh, bh
        mul bx
        add ax, [bdb_reserved_sectors] ; root lba

        push ax
        mov ax, [bdb_dir_entries_count]
        shl ax,5 ; mul by 32 (shift left 5 times)
        xor dx, dx
        
        div word [bdb_bytes_per_sector]

        test dx, dx
        jz .afterRoot
        inc ax
        
    .afterRoot:
        mov cl, al
        pop ax
        mov dl, [ebr_drive_number]
        mov bx, buffer
        
        call disk_read
        
        xor bx, bx
        mov di, buffer
       
    .find_kernel:
        
        mov si, kernel_file
        mov cx, 11
        push di
        repe cmpsb
        pop di
        je .kernel_found

        add di, 32
        inc bx
        cmp bx, [bdb_dir_entries_count]; how much files in root, if not less then kernel not found
        jl .find_kernel

        jmp .kernel_not_found

    .kernel_not_found:
        mov si, diskErrorStr
        call print_str
        hlt
        jmp wait_key_and_reboot

    .kernel_found:
        mov ax, [di+26]
        mov [kernel_cluster], ax

        mov ax, [bdb_reserved_sectors]
        mov bx, buffer
        mov cl, [bdb_sectors_per_fat]
        mov dl, [ebr_drive_number]
        call disk_read

        mov bx, kernel_load_segment
        mov es, bx
        mov bx, kernel_load_offset

    .load_kernel_to_memory:
        
        mov ax, [kernel_cluster]
        add ax, 31
        mov cl, 1
        mov dl, [ebr_drive_number]
        call disk_read
        
        add bx, [bdb_bytes_per_sector]
        mov ax, [kernel_cluster]
        mov cx, 3
        mul cx
        mov cx, 2
        div cx

        mov si, buffer
        add si, ax
        mov ax, [ds:si]

        or dx, dx
        jz .even
    .odd:
        shr ax, 4
        jmp .nextCluster
    .even:
        and ax, 0x0FFF
    .nextCluster:
        cmp ax, 0x0FF8
        jae .finish_reading
        mov [kernel_cluster], ax
        jmp .load_kernel_to_memory

    .finish_reading:
        mov dl, [ebr_drive_number]
        mov ax, kernel_load_segment
        mov ds, ax
        mov es, ax
        jmp kernel_load_segment:kernel_load_offset
        
        cli
        hlt

halt:
    cli
    hlt

floppy_error:

    mov si, diskErrorStr
    call print_str
    hlt
wait_key_and_reboot:
    mov ah, 0
    int 16h                     ; wait for keypress
    jmp 0FFFFh:0
;lba to chs formula:
;C = LBA ÷ (HPC x SPT)
;H = (LBA ÷ SPT) mod HPC
;S = (LBA mod SPT) + 1

lba_to_chs:
    
    push ax
    push dx

    xor dx, dx
    

    div word [bdb_sectors_per_track]
    
    inc dx
    mov cx, dx ; cx = sector

    xor dx, dx
    div word [bdb_heads]
    
    mov dh, dl ; dh=head
    mov ch, al ; ch=cylinder
    shl ah, 6
    or cl, ah
    
    pop ax
    mov dl, al
    pop ax
    
    ret

disk_read:
    push ax
    push bx
    push cx
    push dx
    push di
    
    push cx
    call lba_to_chs
    pop ax

    
    mov ah, 02h
    mov di, 3
.retry:
    
    pusha
    stc
    int 13h
    jnc .done_reading
    
    popa
    call disk_reset
    
    dec di
    test di, di
    jnz .retry
    
.fail:
    
    jmp floppy_error
.done_reading:
    popa
    pop di
    pop dx
    pop cx
    pop bx
    pop ax
    ret

disk_reset:
    pusha
    mov ah, 0
    stc
    int 13h
    jc floppy_error
    popa
    ret

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
    pop bx
    pop ax
    pop si
    ret

kernel_load_segment equ 0x500
kernel_load_offset equ 0x0
kernel_cluster: dw 0
diskErrorStr:   db 'failed!', ENDL, 0
disk_success:   db 'bios', ENDL, 0
kernel_file:    db 'KERNEL  BIN'

times 510-($-$$) db 0
dw 0xAA55

buffer: