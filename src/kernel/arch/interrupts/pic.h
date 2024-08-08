#pragma once

#include "../../clibs/x86.h"
#define PIC1		0x20		
#define PIC2		0xA0		
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)
#define PIC_EOI     0x20

void remap_pic(int offset1, int offset2);
void pic_send_eoi(uint8_t irq);