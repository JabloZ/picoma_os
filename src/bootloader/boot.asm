org 0x7c00

bits 16

jmp short start
nop

;set up fat12
bdb_oem:                    db 'MSWIN4.1'           ; 8 bytes
bdb_bytes_per_sector:       dw 512
bdb_sectors_per_cluster:    db 1
bdb_reserved_sectors:       dw 1
bdb_fat_count:              db 2
bdb_dir_entries_count:      dw 0E0h
bdb_total_sectors:          dw 2880                 ;1.44MiB
bdb_media_descriptor_type:  db 0F0h                 
bdb_sectors_per_fat:        dw 9                    ; 9 sectors
bdb_sectors_per_track:      dw 18
bdb_heads:                  dw 2
bdb_hidden_sectors:         dd 0
bdb_large_sector_count:     dd 0

ebr_drive_number:           db 0                    ; 0x00 floppy, 0x80 hdd, useless
                            db 0                    ; reserved
ebr_signature:              db 29h
ebr_volume_id:              db 12h, 34h, 56h, 78h   ; serial number, value doesn't matter
ebr_volume_label:           db 'PICOMA OS  '        
ebr_system_id:              db 'FAT12   '


print_str:
    mov ah, 0x0e
print_str_loop:
    lodsb               ; loads next character in al
    or al, al
    je done
    int 0x10
    jmp print_str_loop
done:
    ret

start:
    ;data segments
    mov ax, 0   
    mov ds, ax
    mov es, ax
    
    ;set stack
    mov ss, ax
    mov sp, 0x7C00
    push es
    push word .after_start
    retf
.after_start:
    mov [ebr_drive_number], dl
    
    push es
    mov ah, 08h
    int 13h
    jc floppy_error
    pop es

    xor ch, ch
    mov [bdb_sectors_per_track], cx     ; sector count

    inc dh
    mov [bdb_heads], dh     

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
    mov ah, 0x0e
    mov al, 'B'
    int 10h
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
    jmp halt

.kernel_found:
    mov si, disk_success
    call print_str

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
    cmp ax, 0x0ff8
    jae finish_reading
    mov [kernel_cluster], ax
    jmp .load_kernel_to_memory

finish_reading:
    mov si, disk_success
    call print_str

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
    
    call lba_to_chs
    
    pop ax

    mov ah, 02h
    mov di, 3
retry:
    stc
    int 13h
    jnc done_reading
    call disk_reset
    inc di
    test di, di
    jnz retry
fail:
    mov si, diskErrorStr
    call print_str
    hlt
    jmp halt
done_reading:
    popa
                 
    ret

disk_reset:
    pusha
    mov ah, 0
    stc
    int 13h
    jc floppy_error
    popa
    ret

diskErrorStr: db "Couldnt read disk", 0x0D, 0x0A, 0
disk_success: db "hello from bios", 0x0D, 0x0A, 0
kernel_file: db 'KERNEL  BIN'
kernel_cluster dw 0

kernel_load_segment equ 0x2000
kernel_load_offset equ 0

times 510-($-$$) db 0
dw 0xaa55

buffer:
