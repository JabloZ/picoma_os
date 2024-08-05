#pragma once
#include "stdint.h"
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
void load_idt(idt_desc* g_idt);
void set_gate(uint8_t interrupt_num, uint32_t limit, uint16_t selector, uint8_t type_attributes);
idt_entry idt_interrupts[256];
idt_desc g_idt_desc;

