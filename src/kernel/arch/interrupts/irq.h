#pragma once

#include "pic.h"
#include "idt.h"
#include "isr.h"
#include "irq_handle.h"
void enable_interrupts();
void disable_interrupts();

typedef void (*irq_handler)(registers* regs);
void init_irq();
void irq_handler_f(registers* regs);
void irq_wait(int irq_num);
irq_handler g_irq_handler[16];