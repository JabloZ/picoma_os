#pragma once
#include "stdint.h"
typedef enum{
    TASK_GATE=0x5,
    BIT_16_INTERRUPT=0x6,
    BIT_16_TRAP=0x7,
    BIT_32_INTERRUPT=0xE,
    BIT_32_TRAP=0xF,

    IDT_RING_0=0,
    IDT_RING_1=0x20,
    IDT_RING_2=0x40,
    IDT_RING_3=0x60,

    IDT_PRESENT=0x80

} IDT_FLAGS;
void init_idt();
void set_gate(uint8_t interrupt_num, void* limit, uint16_t selector, uint8_t type_attributes);