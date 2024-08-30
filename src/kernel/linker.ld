ENTRY (_start)
OUTPUT_FORMAT(elf32-i386)
SECTIONS
{
    . = 0x00100000;

    _kernel_start = .;
     .prekernel :
    {
        *(.prekernel)
    }
     .prekernel.data :
    {
        *(.prekernel.data)
    }
     .prekernel.bss :
    {
        *(.prekernel.bss)
    }
    .text : AT(ADDR(.text)+0xC0000000)
    {
        *(.text)
    }
    . += 0xC0000000;
    .rodata : AT(ADDR(.rodata)+0xC0000000)
    {
        *(.rodata*)
    }
    
    .data : AT(ADDR(.data)+0xC0000000)
    {
        *(.data)
    }
    
    .bss : AT(ADDR(.bss)+0xC0000000)
    {
        *(COMMON)
        *(.bootstrap_stack)
        *(.bss)
    }
    
    _kernel_end = .;
}
