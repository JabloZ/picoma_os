#include "tss.h"

tss_entry_struct tss_entry;
void usermode(){
    enter_usermode();
    
    //printf("not");
}
void write_tss(gdt_entry* ge){
    uint32_t base=&tss_entry;
    uint32_t limit=sizeof(tss_entry);
    
    ge->limit_low=limit & 0xFFFF;
    ge->base_low=base & 0xFFFF;
    ge->base_middle= (base>>16) & 0xFF;
    ge->access=0x80|0x08|0x01;
    ge->granularity=(limit>>16) &0x0F;
    ge->base_high=(base >> 24) & 0xFF;
    memset(&tss_entry, 0, sizeof(tss_entry));
    tss_entry.ss0=0x10;
    tss_entry.esp0=KERNEL_STACK_TOP;
    flush_tss();
}