#ifndef IRQ_H
#define IRQ_H
#include "pic.h"
#include "idt.h"
#include "isr.h"
#include "irq_handle.h"
#include "../../clibs/stdio.h"
void enable_interrupts();
void disable_interrupts();

typedef void (*irq_handler)(registers* regs);
void init_irq();
void irq_handler_f(registers* regs);
void irq_wait(int irq_num);
__attribute__((aligned(64))) irq_handler g_irq_handler[16];
int capslock_pressed;
int shift_pressed;
#endif