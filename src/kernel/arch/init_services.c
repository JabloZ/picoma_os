#include "init_services.h"
#include "interrupts/idt.h"
#include "gdt/gdt.h"
#include "interrupts/isr.h"
#include "interrupts/irq.h"
#include "../memory/pmm.h"
#include "../memory/page.h"
#include "../memory/vmm.h"
void services_init(){
    init_gdt();
    init_idt();
    init_isr();
    init_irq();
    init_pmm();

    //init_page();
    
    init_vmm();
    
}