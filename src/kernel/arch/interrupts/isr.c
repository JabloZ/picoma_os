
#include "isr.h"



char *exceptionMessages[] = {
    "Division by Zero",
    "Debug",
    "Non-Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bat TSS",
    "Segment not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};



void enable_gate(uint8_t interrupt){
    idt_interrupts[interrupt].type_attributes |= IDT_PRESENT;
}
void disable_gate(uint8_t interrupt){
    idt_interrupts[interrupt].type_attributes &= ~IDT_PRESENT;
}
void init_isr(){
    
    set_gate(0, (uint32_t)&isr_0, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(1, (uint32_t)&isr_1, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(2, (uint32_t)&isr_2, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(3, (uint32_t)&isr_3, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(4, (uint32_t)&isr_4, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(5, (uint32_t)&isr_5, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(6, (uint32_t)&isr_6, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(7, (uint32_t)&isr_7, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(8, (uint32_t)&isr_8, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(9, (uint32_t)&isr_9, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(10, (uint32_t)&isr_10, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(11, (uint32_t)&isr_11, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(12, (uint32_t)&isr_12, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(13, (uint32_t)&isr_13, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(14, (uint32_t)&isr_14, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(15, (uint32_t)&isr_15, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(16, (uint32_t)&isr_16, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(17, (uint32_t)&isr_17, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(18, (uint32_t)&isr_18, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(19, (uint32_t)&isr_19, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(20, (uint32_t)&isr_20, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(21, (uint32_t)&isr_21, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(22, (uint32_t)&isr_22, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(23, (uint32_t)&isr_23, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(24, (uint32_t)&isr_24, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(25, (uint32_t)&isr_25, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(26, (uint32_t)&isr_26, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(27, (uint32_t)&isr_27, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(28, (uint32_t)&isr_28, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(29, (uint32_t)&isr_29, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(30, (uint32_t)&isr_30, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(31, (uint32_t)&isr_31, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(32, (uint32_t)&isr_32, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(33, (uint32_t)&isr_33, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(34, (uint32_t)&isr_34, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(35, (uint32_t)&isr_35, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(36, (uint32_t)&isr_36, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(37, (uint32_t)&isr_37, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(38, (uint32_t)&isr_38, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(39, (uint32_t)&isr_39, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(40, (uint32_t)&isr_40, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(41, (uint32_t)&isr_41, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(42, (uint32_t)&isr_42, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(43, (uint32_t)&isr_43, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(44, (uint32_t)&isr_44, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(45, (uint32_t)&isr_45, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(46, (uint32_t)&isr_46, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(47, (uint32_t)&isr_47, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);
    set_gate(48, (uint32_t)&isr_48, 0x08, IDT_RING_0 | BIT_32_INTERRUPT);

    for(uint8_t i=0; i<32; i++){
        enable_gate(i);
    }
    
    load_idt(&g_idt_desc);
    
}
    
void  __attribute__((cdecl)) exception_handler(registers* frame){
    if (isr_h[frame->interrupt]!=NULL){
        isr_h[frame->interrupt](frame);
    }
    else{
        printf("\nunexpected interrupt: %d\n", frame->interrupt);
        printf("ERROR MESSAGE: %s",exceptionMessages[frame->interrupt]);
        panic();}

    //return;
}
