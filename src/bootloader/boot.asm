
[org 0x7c00]
KERNEL_LOCATION equ 0x1000

BITS 16

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

start: 
   jmp main

print_str:
    
    mov ah, 0x0e
    print_str_loop:
        mov al, [si]
        inc si
        cmp al, 0
        je .done
        int 0x10
        jmp print_str_loop
.done:
    ret


main:
    ;data segments
    mov ax, 0   
    mov ds, ax
    mov es, ax
    
    ;set stack
    mov ss, ax
    mov sp, 0x7C00

    
    mov [ebr_drive_number], dl ;set drive
    mov ax, 1 ; read second sector
    mov cl, 1 ; read one sector
    mov bx, 0x7e00
    mov si, disk_success
    call print_str
    cli 
    hlt

    .halt:
        cli
        hlt

floppy_error:
    mov si, diskErrorStr
    call print_str
    jmp wait_key_and_reboot

wait_key_and_reboot:
    mov ah, 0
    int 16h                     ; wait for keypress
    jmp 0FFFFh:0                ; jump to beginning of BIOS, should reboot

.halt:
    cli                         ; disable interrupts, this way CPU can't get out of "halt" state
    hlt


;lba to chs formula:
;C = LBA ÷ (HPC x SPT)
;H = (LBA ÷ SPT) mod HPC
;S = (LBA mod SPT) + 1
lba_to_chs:
    push ax
    push bx
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
    
    int 13h
    .retry:
        pusha
        stc
        int 13h
        jnc .done
        popa
        call disk_reset
        inc di
        test di, di
        jnz .retry

    .fail:
        mov si, diskErrorStr
        call print_str
    .done:
        popa
        pop di
        pop dx
        pop cx
        pop bx
        pop ax                             ; restore registers modified
        ret


disk_reset:
    pusha
    mov ah, 0
    stc
    int 13h
    jc floppy_error
    popa
    ret


diskErrorStr: db "Couldn't read disk", 0
disk_success: db "hello from the bios i work!", 0

times 510-($-$$) db 0
dw 0xaa55


