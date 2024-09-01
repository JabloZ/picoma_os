
#include "irq.h"
#include "../../clibs/stdio.h"
#define REMAP_OFFSET 0x20
void enable_interrupts(){
    __asm__ volatile ("sti; hlt");
}

void disable_interrupts(){
    __asm__ volatile ("cli; hlt");
}
void handler_irq_0(){
    ;
}

void init_irq(){
    
    g_irq_handler[0]=handler_irq_0;
    g_irq_handler[1]=handler_irq_1;
    
    remap_pic(REMAP_OFFSET, REMAP_OFFSET+8);
    
    for (uint8_t i=0; i<16; i++){
        isr_h[REMAP_OFFSET+i]=irq_handler_f;
        enable_gate(REMAP_OFFSET+i);
    }
    enable_interrupts();
    
}
void irq_handler_f(registers* frame){
    int irq_num=frame->interrupt-REMAP_OFFSET;
    if (g_irq_handler[irq_num]!=NULL){
        g_irq_handler[irq_num](frame);
    }
    else{
        printf("\ninterrupt num: %d",irq_num);
    }
    
    pic_send_eoi(frame->interrupt-REMAP_OFFSET);
    
}
