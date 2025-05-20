#include "main.h"


void __attribute__((section(".entry"))) _start(uint16_t boot_drive, global_mem_info* g_memory_regs_info_boot)
{
    
  
    
   //__asm__ volatile("hlt;");
    clear_screen();
    
    g_memory_regs_info=g_memory_regs_info_boot;
    //print_regions();

    init_vals();
    
    services_init();
    
    printf("________________________________________________________________________________");
    printf("|                            X86 PICOMA OS: v0.0.2                             |");
    printf("________________________________________________________________________________\n");
    printf(">");
    print_pmm();
     void* kernel_stack_top=(void*)0xC03FFFFF;
    
    __asm__ volatile (
    "mov %0, %%esp\n"
    "mov %0, %%ebp\n"
    :
    : "r" (kernel_stack_top)
    );
    
    //vmm_alloc_page_4kb(0xC0805000);
    //vmm_alloc_page_4kb(0xC0806000);
    //print_pmm();
    //vmm_unmap_page_4kb(0xC0805000);
    //print_pmm();
    //vmm_memory_status();
    
    init_fdc();
    init_opofs(0);
    
    file_entry save_f;
    file_entry file_e;
    

   
    //test
    
   
    //memory_free(temp);
    //memory_free(temp2);
    //print_pmm();
    /*
    uint8_t* buf=mem_allocate(40950);
    memory_free(buf);
    uint8_t* buf2=mem_allocate(20550);
    memory_free(buf2);
    uint8_t* buf3=mem_allocate(45110);
    memory_free(buf3);
    uint8_t* buf4=mem_allocate(2);
    memory_free(buf4);*/
    
    print_pmm();
    
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
    
    //read_elf("testelf.bin");

    //usermode();
    
   
    //handler_irq_1();
    write_tss(&g_gdt_entries[5]);
    //printf("~/>");
    //execute_or_recognize_command();
    
  
    
    //printf("filetest    txt");
   //creat_file_opofs(&root_dir, 3, "filetest.txt", 900,0,0);
end:
    while(1);
}
void print_regions(){
    for (int i=0; i<g_memory_regs_info->region_num; i++){
         //printf("E820: base=%p length=%p type=%p acpi=%p\n", mem_r.base_low, mem_r.region_low, mem_r.region_type, mem_r.acpi);
        printf("base: 0x%p, length: 0x%p, type: %p\n",g_memory_regs_info->memory_regions[i].base_low, g_memory_regs_info->memory_regions[i].region_low, g_memory_regs_info->memory_regions[i].region_type);
    }
}
