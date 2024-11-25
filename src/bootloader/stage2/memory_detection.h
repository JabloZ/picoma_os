#pragma once
#include "memory_params.h"
#include "x86.h"
#include "stdio.h"
#include "stdint.h"

memory_region g_mem_regs[256];
void detect_mem();