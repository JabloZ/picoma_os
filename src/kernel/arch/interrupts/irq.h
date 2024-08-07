#pragma once

#include "pic.h"
#include "idt.h"
#include "isr.h"

void enable_interrupts();
void disable_interrupts();

typedef void (*irq_handler)(registers* regs);
void init_irq();
void irq_handler_f(registers* regs);
irq_handler g_irq_handler[16];