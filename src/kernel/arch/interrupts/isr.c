#include "stdint.h"
#include "stdbool.h"
#include "idt.h"
#include "stdio.h"
//#include "gdt/gdt.h" doesnt work idk why
extern void* isr_table[];

void init_isr(){
    for(uint8_t i=0; i<32; i++){
        set_gate(i, isr_table[i], 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    }
}
void exception_handler(){
    printf("exception handler");
}