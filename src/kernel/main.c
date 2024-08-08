#include "clibs/stdint.h"
#include "clibs/stdio.h"
#include "clibs/memory.h"
#include "arch/init_services.h"
#include "arch/interrupts/irq.h"
#include "../glibs/memory_params.h"
extern uint8_t __bss_start;
extern uint8_t __end;

void print_regions(){
    for (int i=0; i<g_memory_regs_info->region_num; i++){
        printf("base: 0x%p, length: 0x%p, type: %p\n",g_memory_regs_info->memory_regions[i].base_low, g_memory_regs_info->memory_regions[i].region_low, g_memory_regs_info->memory_regions[i].region_type);
    }
}
void __attribute__((section(".entry"))) _start(uint16_t bootDrive)
{
    memset(&__bss_start, 0, (&__end) - (&__bss_start)); //zero uninitialized data

    clear_screen();
    
    services_init();
    printf("memory regs: %d",g_memory_regs_info->region_num);
    //print_regions();
    //int b=2/0;
    
end:
    while(1);
}