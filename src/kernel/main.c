#include "clibs/stdint.h"
#include "clibs/stdio.h"
#include "clibs/memory.h"
#include "arch/init_services.h"
#include "arch/interrupts/irq.h"
#include "../glibs/memory_params.h"
#include "memory/page.h"


void print_regions(){
    for (int i=0; i<g_memory_regs_info->region_num; i++){
        printf("base: 0x%p, length: 0x%p, type: %p\n",g_memory_regs_info->memory_regions[i].base_low, g_memory_regs_info->memory_regions[i].region_low, g_memory_regs_info->memory_regions[i].region_type);
    }
}
void _start()
{
    
    clear_screen();
    
    services_init();
    printf("________________________________________________________________________________");
    printf("|                            X86 PICOMA OS: v0.0.1                             |");
    printf("________________________________________________________________________________\n");
    //print_regions();
    //int b=2/0;
    printf(">");
    create_page_table();
    uint32_t actual_mem=0x400000;

    uint32_t max_mem= g_memory_regs_info->memory_regions[g_memory_regs_info->region_num-1].base_low+g_memory_regs_info->memory_regions[g_memory_regs_info->region_num-1].region_low;
    
    while (max_mem>actual_mem+0x400000){
        
        create_page_table();
        actual_mem+=0x400000;
    }
    /*
    uint32_t* test_address = (uint32_t*)0x2000;
    map_page((void*)0x800000, (void*)test_address);  // mapuj 0x2000 do fizycznego 0x100000

    *test_address = 0xBACABACA;

    if (*test_address == 0xBACABACA){
        printf("paging and memory allocation successful \n");
    }*/
end:
    while(1);
}