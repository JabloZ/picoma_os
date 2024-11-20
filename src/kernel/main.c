#include "clibs/stdint.h"
#include "clibs/stdio.h"
#include "clibs/memory.h"
#include "arch/init_services.h"
#include "arch/interrupts/irq.h"
#include "arch/interrupts/idt.h"
#include "arch/disk.h"
#include "arch/drivers/fs/opofs.h"
#include "arch/drivers/fdc.h"
#include "arch/drivers/fs/fat.h"
#include "../glibs/memory_params.h"
#include "memory/page.h"
#include "memory/kalloc.h"
#include "memory/pmm.h"
void __attribute__((section(".entry"))) _start(uint16_t boot_drive)
{
   //__asm__ volatile("hlt;");
    clear_screen();
    //print_regions();
    init_vals();
    services_init();
    printf("________________________________________________________________________________");
    printf("|                            X86 PICOMA OS: v0.0.2                             |");
    printf("________________________________________________________________________________\n");
    printf(">");
    print_pmm();
    
    //vmm_alloc_page_4kb(0xC0805000);
    //vmm_alloc_page_4kb(0xC0806000);
    //print_pmm();
    //vmm_unmap_page_4kb(0xC0805000);
    //print_pmm();
    vmm_memory_status();
    init_fdc();
    init_opofs(0);

    uint8_t temp_buf[512];
    fdc_read_sector(0,1,temp_buf,0,sector_read);
    //print_regions();
    //init_fat();
    //open_fat(0,"test.txt");
    //fdc_read_track(0, 0);
   /*
    uint8_t sector_s[SECTOR_SIZE*2];
    fdc_read_sectors(0, 0, 2, sector_s);
   
    for (int i=0; i<SECTOR_SIZE*2; i++){
    printf("%c",sector_s[i]);}*/
   
    
    //fdc_write_sector(0,991,&buf,0,sector_write);
    setup_global_file_info_table();
    printf(">");
   
    //printf("filetest    txt");
   //create_file_opofs(&root_dir, 3, "filetest.txt", 900,0,0);
    
    
end:
    while(1);
}
void print_regions(){
    for (int i=0; i<g_memory_regs_info->region_num; i++){
         //printf("E820: base=%p length=%p type=%p acpi=%p\n", mem_r.base_low, mem_r.region_low, mem_r.region_type, mem_r.acpi);
        printf("base: 0x%p, length: 0x%p, type: %p\n",g_memory_regs_info->memory_regions[i].base_low, g_memory_regs_info->memory_regions[i].region_low, g_memory_regs_info->memory_regions[i].region_type);
    }
}
