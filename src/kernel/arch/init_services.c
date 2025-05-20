#include "init_services.h"

void services_init(){
   init_gdt();
    init_idt();
    init_isr();
    
    
    init_irq();
    init_pmm();
    
    init_page();
    
    init_kalloc();
    
}