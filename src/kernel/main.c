#include "stdint.h"
#include "stdio.h"
#include "memory.h"
#include "arch/hal.h"
extern uint8_t __bss_start;
extern uint8_t __end;

void __attribute__((section(".entry"))) _start(uint16_t bootDrive)
{
    memset(&__bss_start, 0, (&__end) - (&__bss_start)); //zero uninitialized data
    clear_screen();
    hal_init();
    printf("Hello kernel\n");
    //int b=2/0;
end:
    while(1);
}