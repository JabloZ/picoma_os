#ifndef MEMORY_DETECTION_H
#define MEMORY_DETECTION_H
#include "memory_params.h"
#include "x86.h"
#include "stdio.h"
#include "stdint.h"

extern memory_region g_mem_regs[256];
void detect_mem();
#endif