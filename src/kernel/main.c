#include "clibs/stdint.h"
#include "clibs/stdio.h"
#include "clibs/memory.h"
#include "arch/init_services.h"
#include "arch/interrupts/irq.h"
#include "../glibs/memory_params.h"
#include "memory/page_entry.h"
#include "memory/page.h"
void print_string(char *str)
{
  uint8_t* vidmem = (uint8_t *) 0xc01b8000;
  while(*str)
  {
    *vidmem++ = *str++;
    *vidmem++ = 0x7;
  }
}
void kernel_main()
{
  
    //
    //int b=2;

    print_string("hello, world!");
    //printf("po: %d",b);
    clear_screen();
    __asm__ volatile("hlt");
    services_init();
    printf("________________________________________________________________________________");
    printf("|                            X86 PICOMA OS: v0.0.1                             |");
    printf("________________________________________________________________________________\n");
    print_regions();
    //int b=2/0;
    printf(">");
    
    uint32_t max_mem= g_memory_regs_info->memory_regions[g_memory_regs_info->region_num-1].base_low+g_memory_regs_info->memory_regions[g_memory_regs_info->region_num-1].region_low;
    //int i=0;
   
end:
    while(1);
}
void print_regions(){
    for (int i=0; i<g_memory_regs_info->region_num; i++){
        printf("base: 0x%p, length: 0x%p, type: %p\n",g_memory_regs_info->memory_regions[i].base_low, g_memory_regs_info->memory_regions[i].region_low, g_memory_regs_info->memory_regions[i].region_type);
    }
}
