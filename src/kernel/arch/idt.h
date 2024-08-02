#pragma once
#include "stdint.h"

void init_idt();
void set_gate(uint8_t interrupt_num, uint32_t limit, uint16_t selector, uint8_t type_attributes);