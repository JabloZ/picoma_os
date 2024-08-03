#include "stdint.h"
#include "idt.h"

typedef struct{
    uint16_t offset_1;
    uint16_t selector;
    uint8_t zero;
    uint8_t type_attributes;
    uint16_t offset_2;
} __attribute__((packed)) idt_entry;

typedef struct{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_desc;



idt_entry idt_interrupts[256];

void set_gate(uint8_t interrupt_num, void* limit, uint16_t selector, uint8_t type_attributes){
    idt_entry* descriptor=&idt_interrupts[interrupt_num];

    descriptor->offset_1=((uint32_t)limit & 0xFFFF);
    descriptor->selector=selector;
    descriptor->zero=0; 
    descriptor->type_attributes=type_attributes; 
    descriptor->offset_2=((uint32_t)limit>>16 & 0xFFFF);  

}
void init_idt(){
    
    idt_desc g_idt_desc={sizeof(idt_interrupts)-1,idt_interrupts};

    load_idt(&g_idt_desc);
    
}
