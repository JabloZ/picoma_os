
#include "idt.h"



void set_gate(uint8_t interrupt_num, uint32_t limit, uint16_t selector, uint8_t type_attributes){
    idt_entry* descriptor=&idt_interrupts[interrupt_num];

    descriptor->offset_1=((uint32_t)limit) & 0xFFFF;
    descriptor->selector=selector;
    descriptor->zero=0; 
    descriptor->type_attributes=type_attributes; 
    descriptor->offset_2=((uint32_t)limit>>16) & 0xFFFF;  

}

void init_idt(){
   
    g_idt_desc.limit=sizeof(idt_interrupts)-1;
    g_idt_desc.base=(uint32_t)&idt_interrupts;
    
}

