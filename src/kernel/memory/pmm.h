#ifndef PMM_H
#define PMM_H
#include "../clibs/stdbool.h"
#include "../clibs/stdint.h"
#include "../memory_params.h"

#define PAGE_SIZE 4096

#define total_pages 1048576
uint8_t physical_mem[total_pages];
uint32_t last_alloc;
void init_pmm();
void* pmm_alloc();
void pmm_free();
uint32_t pmm_alloc_addr();
void print_pmm();
#endif