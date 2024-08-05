
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
