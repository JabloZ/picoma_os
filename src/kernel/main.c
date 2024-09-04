#include "clibs/stdint.h"
#include "clibs/stdio.h"
#include "clibs/memory.h"
#include "arch/init_services.h"
#include "arch/interrupts/irq.h"
#include "arch/interrupts/idt.h"
#include "arch/disk.h"
#include "../glibs/memory_params.h"
#include "memory/page.h"
#include "memory/kalloc.h"
#include "memory/pmm.h"
void __attribute__((section(".entry"))) _start()
{

    clear_screen();
    services_init();
    printf("________________________________________________________________________________");
    printf("|                            X86 PICOMA OS: v0.0.1                             |");
    printf("________________________________________________________________________________\n");
    printf(">");
    print_pmm();
    //vmm_alloc_page_4kb(0xC0805000);
    //vmm_alloc_page_4kb(0xC0806000);
    //print_pmm();
    //vmm_unmap_page_4kb(0xC0805000);
    //print_pmm();
    vmm_memory_status();
    //uint8_t* ata_membuf;
    //ata_lba_read(0,1,ata_membuf);

end:
    while(1);
}
void print_regions(){
    for (int i=0; i<g_memory_regs_info->region_num; i++){
        printf("base: 0x%p, length: 0x%p, type: %p\n",g_memory_regs_info->memory_regions[i].base_low, g_memory_regs_info->memory_regions[i].region_low, g_memory_regs_info->memory_regions[i].region_type);
    }
}
