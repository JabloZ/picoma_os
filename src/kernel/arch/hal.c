#include "hal.h"
#include "interrupts/idt.h"
#include "gdt/gdt.h"
#include "interrupts/isr.h"

void hal_init(){
    init_gdt();
    init_idt();
    init_isr();
}