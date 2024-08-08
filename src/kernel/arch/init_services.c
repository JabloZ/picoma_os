#include "init_services.h"
#include "interrupts/idt.h"
#include "gdt/gdt.h"
#include "interrupts/isr.h"
#include "interrupts/irq.h"

void services_init(){
    init_gdt();
    init_idt();
    init_isr();
    init_irq();
}