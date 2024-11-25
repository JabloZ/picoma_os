#ifndef INIT_SERVICES_H
#define INIT_SERVICES_H

#include "interrupts/idt.h"
#include "gdt/gdt.h"
#include "interrupts/isr.h"
#include "interrupts/irq.h"
#include "../memory/pmm.h"
#include "../memory/page.h"
#include "../memory/kalloc.h"
void init_services();
#endif