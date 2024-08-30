#pragma once
#include "../clibs/stdbool.h"
#include "../clibs/stdint.h"
#include "../../glibs/memory_params.h"

#define PAGE_SIZE 4096

#define total_pages 4096
uint8_t physical_bitmap[4096];

void init_pmm();
void* pmm_alloc();