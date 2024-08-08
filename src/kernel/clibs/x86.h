#pragma once
#include "stdint.h"
#include "stdbool.h"
void outb(uint16_t port, uint8_t data);
static inline void io_wait(void)
{
    outb(0x80, 0);
}