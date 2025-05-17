#ifndef ISR_H
#define ISR_H
#include "../../clibs/stdint.h"
#include "../../clibs/stdint.h"
#include "../../memory_params.h"
//#include "../drivers/fdc.h"
#include "idt.h"
typedef struct{
      uint32_t ds;                                            
    uint32_t edi, esi, ebp, useless, ebx, edx, ecx, eax;  
    uint32_t interrupt, error;                            
    uint32_t eip, cs, eflags, esp, ss;
} __attribute__((packed)) registers;

typedef void(*isr_handler)(registers*);

extern void isr_0();
extern void isr_1();
extern void isr_2();
extern void isr_3();
extern void isr_4();
extern void isr_5();
extern void isr_6();
extern void isr_7();
extern void isr_8();
extern void isr_9();
extern void isr_10();
extern void isr_11();
extern void isr_12();
extern void isr_13();
extern void isr_14();
extern void isr_15();
extern void isr_16();
extern void isr_17();
extern void isr_18();
extern void isr_19();
extern void isr_20();
extern void isr_21();
extern void isr_22();
extern void isr_23();
extern void isr_24();
extern void isr_25();
extern void isr_26();
extern void isr_27();
extern void isr_28();
extern void isr_29();
extern void isr_30();
extern void isr_31();
extern void isr_32();
extern void isr_33();
extern void isr_34();
extern void isr_35();
extern void isr_36();
extern void isr_37();
extern void isr_38();
extern void isr_39();
extern void isr_40();
extern void isr_41();
extern void isr_42();
extern void isr_43();
extern void isr_44();
extern void isr_45();
extern void isr_46();
extern void isr_47();
extern void isr_48();
extern void isr6();
void init_isr();
void enable_gate(uint8_t interrupt);
void disable_gate(uint8_t interrupt);
void exception_handler(registers* frame);
__attribute__((aligned(64))) isr_handler isr_h[256];
#endif